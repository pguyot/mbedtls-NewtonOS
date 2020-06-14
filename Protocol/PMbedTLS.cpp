// =========== Header ===========
// File:                PMbedTLS.cpp
// Project:             mbed TLS for NewtonOS
// Written by:          Paul Guyot (pguyot@kallisys.net)
//
// Created on:          10/05/2020
// Internal version:    1
//
// Copyright:           © 2020 by Paul Guyot.
//                      All rights reserved worldwide.
// ===========

// =========== Change History ===========
// 10/05/2020   v1  [PG]    Creation of the file
// ===========

#include "PMbedTLS.impl.h"

// NewtonOS
#include <Newton.h>
#include <NSandDDKIncludes.h>
#include <NewtonScript.h>
#include <NameServer.h>
#include <NewtonExceptions.h>
#include <OSErrors.h>
#include <NewtonErrors.h>

// mbedtls
#include "mbedtls/certs.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ssl_cache.h"
#include "mbedtls/version.h"
#include "mbedtls/x509.h"

#include "PMbedTLSConnectionEnd.impl.h"
#include "mbedTLSEvents.h"
#include "NewtonBindings.h"

// Only available on NewtonOS 2.1
#define ROM_GetSerialNumber MAKEMAGICPTR(846)

DeclareException(exMbedTLSException, exRootException);
DefineException(exMbedTLSException, evt.ex.org.mbed.tls);
DeclareException(exFrames, exRootException);
DeclareException(exStoreError, exFrames);
DeclareException(exInterpreter, exFrames);
DeclareException(exInterpreterWithFrameData, exInterpreter);

struct CObject {
    void*               vtable;
    void*               ptr;
    CObjectBinaryProc   destructor;
    CObjectBinaryProc   marker;
    CObjectBinaryProc   updated;
    ULong               flags;
};

struct CObjectBin {
    ObjHeader   header;
    Ref         binClass;
    CObject     binary;
};

#define WITH_COBJECT_PTR(binaryObj, ptrType, ptrName) \
    unwind_protect { \
        ptrType* ptrName = (ptrType*) LockedBinaryPtr(binaryObj);

#define END_WITH_COBJECT_PTR(binaryObj) \
    END_WITH_LOCKED_BINARY(binaryObj)

#define WITH_COBJECT_PTR_OPT(binaryObj, ptrType, ptrName) \
    unwind_protect { \
        ptrType* ptrName = NULL; \
        if (!ISNIL(binaryObj)) ptrName = (ptrType*) LockedBinaryPtr(binaryObj);

#define END_WITH_COBJECT_PTR_OPT(binaryObj) \
        } \
        on_unwind {             \
            if (!ISNIL(binaryObj)) { \
                UnlockRefArg(binaryObj);     \
            } \
        } \
        end_unwind;

#define WITH_LOCKED_BINARY_MAYBE_STRING(binaryObj, ptrName, lenName) \
    bool ptrName ## Allocated = false; \
    const unsigned char* ptrName = NULL; \
    unwind_protect { \
        long lenName = Length(binaryObj); \
        void* ptrName ## Ptr = LockedBinaryPtr(binaryObj); \
        if (IsString(binaryObj)) { \
            ptrName ## Allocated = true; \
            lenName = lenName / 2; \
            ptrName = (const unsigned char*) ::NewPtr(lenName); \
            for (int i = 0; i < lenName; i++) { \
                ((unsigned char*) ptrName)[i] = ((Character*) ptrName ## Ptr)[i]; \
            } \
        } else { \
            ptrName = (const unsigned char*) ptrName ## Ptr; \
        }

#define END_WITH_LOCKED_BINARY_MAYBE_STRING(binaryObj, ptrName)  \
        } \
        on_unwind {             \
            if (ptrName ## Allocated) { \
                ::DisposePtr((Ptr) ptrName); \
            } \
            UnlockRefArg(binaryObj);     \
        } \
        end_unwind;

// Soup functions
Ref GetUnionSoup(const RefVar&);
Ref Query(const RefVar&, const RefVar&);
Ref CursorEntry(const RefVar&);
Ref CursorMove(const RefVar&, long);

// ======================================================================== //
//  Data structures
// ======================================================================== //

// Because of garbage collector, we keep callback functions with RefStructs.

struct SSLContextWithCallbacks {
    mbedtls_ssl_context context;
    RefStruct           sendFunction;
    RefStruct           recvFunction;
    RefStruct           setTimerFunction;
    RefStruct           getTimerFunction;
};

struct SSLConfigWithCallbacks {
    mbedtls_ssl_config  config;
    RefStruct           sniFunction;
    RefStruct           ticketWriteFunction;
    RefStruct           ticketParseFunction;
    RefStruct           verifyFunction;
    PMbedTLS*           obj;
};


// ======================================================================== //
//  Prototypes
// ======================================================================== //

PROTOCOL_IMPL_SOURCE_MACRO(PMbedTLS)            // Magic stuff, do not touch

#define DISPOSE_PTR_PUBLIC_JT 0x01be10b4

// ------------------------------------------------------------------------- //
//  * New()
// ------------------------------------------------------------------------- //
PMbedTLSInterface*
PMbedTLS::New()
{
    mHardwarePollEventHandler = new TMbedTLSHardwarePollEventHandler();
    mNSSeedReadEventHandler = new TMbedTLSNVSeedReadEventHandler();
    mNSSeedWriteEventHandler = new TMbedTLSNVSeedWriteEventHandler();
    mGetGMTDeltaEventHandler = new TMbedTLSGetGMTDeltaEventHandler();
    mGetSerialNumberEventHandler = new TMbedTLSGetSerialNumberEventHandler();
    mListRootCertificatesEventHandler = new TMbedTLSListRootCertificatesEventHandler();
    mGetRootCertificateEventHandler = new TMbedTLSGetRootCertificateEventHandler();

    mPointers = new RefStruct(NSCallGlobalFn(SYM(NewWeakArray), MAKEINT(10)));
    return this;
}

// ------------------------------------------------------------------------- //
//  * Delete()
// ------------------------------------------------------------------------- //
void
PMbedTLS::Delete()
{
    // Always remove connection end.
    (void) RemoveConnectionEnd();
    long len = mPointers->Length();
    for (int index = 0; index < len; index++) {
        RefVar cobjectBin = mPointers->GetArraySlot(index);
        if (!ISNIL(cobjectBin)) {
            FreeCObjectBinary(cobjectBin);
        }
    }

    delete mPointers;

    delete mHardwarePollEventHandler;
    delete mNSSeedReadEventHandler;
    delete mNSSeedWriteEventHandler;
    delete mGetGMTDeltaEventHandler;
    delete mGetSerialNumberEventHandler;
    delete mListRootCertificatesEventHandler;
    delete mGetRootCertificateEventHandler;
}

// ------------------------------------------------------------------------ //
// * GetRootCertificate(const char*, unsigned char*, size_t*)
// ------------------------------------------------------------------------ //
int
PMbedTLS::GetRootCertificate(const char* name, unsigned char* outBuf, size_t* ioSize)
{
    int result = 0;
    newton_try
    {
        RefVar soupAndStoreName = MakeString("CACertificates:Kallisys");
        RefVar unionSoup = GetUnionSoup(soupAndStoreName);
        RefVar query = AllocateFrame();
        SetFrameSlot(query, SYM(type), SYM(index));
        SetFrameSlot(query, SYM(indexPath), SYM(tag));
        RefVar nameStr = MakeString(name);
        SetFrameSlot(query, SYM(beginKey), nameStr);
        SetFrameSlot(query, SYM(endKey), nameStr);
        RefVar entry = NILREF;
        if (!ISNIL(unionSoup)) {
            RefVar cursor = Query(unionSoup, query);
            newton_try {
                entry = CursorEntry(cursor);
            } newton_catch (exStoreError) {
                // Ignore store errors (bad index in Union soup)
            } end_try;
        }
        if (ISNIL(entry)) {
            // Try with the package store.
            RefVar packageStore = NSCallGlobalFn(SYM(GetPackageStore), soupAndStoreName);
            RefVar packageSoup = NSSend(packageStore, SYM(GetSoup), soupAndStoreName);
            RefVar cursor = Query(packageSoup, query);
            entry = CursorEntry(cursor);
        }
        if (ISNIL(entry)) {
            return -1;
        }
        RefVar trust = GetFrameSlot(entry, SYM(trust));
        if (ISNIL(trust)) {
            return -1;
        }
        RefVar cert = GetFrameSlot(entry, SYM(pem));
        if (ISNIL(cert)) {
            cert = GetFrameSlot(entry, SYM(der));
        }
        if (!IsBinary(cert)) {
            return -1;
        }
        ULong certLen = Length(cert);
        if (ioSize) {
            if (*ioSize >= certLen) {
                WITH_LOCKED_BINARY(cert, certPtr)
                    ::BlockMove((const unsigned char*) certPtr, outBuf, certLen);
                END_WITH_LOCKED_BINARY(cert)
                *ioSize = certLen;
            }
        }
        result = certLen;
    } newton_catch (exInterpreterWithFrameData) {
        result = -1;
    } newton_catch (exFrames) {
        result = (int) CurrentException()->data;
        if (result > 0) {
            result = -1;
        }
    } newton_catch_all {
        result = -1;
    }
    end_try;
    return result;
}

// ------------------------------------------------------------------------ //
// * ListRootCertificates(char*, size_t*)
// ------------------------------------------------------------------------ //
int
PMbedTLS::ListRootCertificates(char* outBuf, size_t* ioSize)
{
    char* bufCursor = outBuf;
    char* endCursor;
    if (ioSize) {
        endCursor = outBuf + *ioSize;
    }
    ULong totalSize = 0;
    ULong writtenSize = 0;
    newton_try
    {
        RefVar soupAndStoreName = MakeString("CACertificates:Kallisys");
        RefVar packageStore = NSCallGlobalFn(SYM(GetPackageStore), soupAndStoreName);
        RefVar packageSoup = NSSend(packageStore, SYM(GetSoup), soupAndStoreName);
        RefVar cursor = Query(packageSoup, NILREF);
        RefVar entry = CursorEntry(cursor);
        RefVar unionSoup = GetUnionSoup(soupAndStoreName);
        RefVar query = AllocateFrame();
        SetFrameSlot(query, SYM(type), SYM(index));
        SetFrameSlot(query, SYM(indexPath), SYM(tag));
        while (!ISNIL(entry)) {
            RefVar tag = GetFrameSlot(entry, SYM(tag));
            RefVar unionEntry = NILREF;
            if (!ISNIL(unionSoup)) {
                SetFrameSlot(query, SYM(beginKey), tag);
                SetFrameSlot(query, SYM(endKey), tag);
                RefVar unionCursor = Query(unionSoup, query);
                unionEntry = CursorEntry(unionCursor);
            }
            if (ISNIL(unionEntry)) {
                UniChar* tagStr = GetCString(tag);
                while (1) {
                    char c = (char) ((unsigned char) *tagStr++);
                    if (bufCursor != NULL) {
                        *bufCursor++ = c;
                        writtenSize++;
                        if (bufCursor == endCursor) {
                            bufCursor = NULL;
                        }
                    }
                    totalSize++;
                    if (c == 0) break;
                }
            }
            entry = CursorMove(cursor, 1);
        }

        if (!ISNIL(unionSoup)) {
            cursor = Query(unionSoup, NILREF);
            entry = CursorEntry(cursor);
            while (!ISNIL(entry)) {
                RefVar tag = GetFrameSlot(entry, SYM(tag));
                UniChar* tagStr = GetCString(tag);
                while (1) {
                    char c = (char) ((unsigned char) *tagStr++);
                    if (bufCursor != NULL) {
                        *bufCursor++ = c;
                        writtenSize++;
                        if (bufCursor == endCursor) {
                            bufCursor = NULL;
                        }
                    }
                    totalSize++;
                    if (c == 0) break;
                }
                entry = CursorMove(cursor, MAKEINT(1));
            }
        }
    }
    newton_catch (exFrames)
    {
        return (int) CurrentException()->data;
    }
    newton_catch_all
    {
        return -1;
    }
    end_try;
    if (ioSize) {
        *ioSize = writtenSize;
    }
    return totalSize;
}

// ------------------------------------------------------------------------- //
//  * GetVersion()
// ------------------------------------------------------------------------- //
Ref
PMbedTLS::GetVersion()
{
    char* versionStr = ::NewPtr(9);
    mbedtls_version_get_string(versionStr);
    RefVar result = ::MakeString( versionStr );
    ::DisposePtr(versionStr);
    return result;
}

// ------------------------------------------------------------------------ //
// * SSLListCiphersuites()
// ------------------------------------------------------------------------ //
// Return the list of supported cipher suites.

Ref
PMbedTLS::SSLListCiphersuites()
{
    const int* ciphersuites = mbedtls_ssl_list_ciphersuites();
    int count = 0;
    while (ciphersuites[count]) count++;
    RefVar result = AllocateArray(SYM(array), count);
    for (int index = 0; index < count; index++) {
        RefVar name = ::MakeString(mbedtls_ssl_get_ciphersuite_name(ciphersuites[index]));
        SetArraySlot(result, index, name);
    }
    return result;
}

// ------------------------------------------------------------------------ //
// * EntropyNew()
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::EntropyNew()
{
    mbedtls_entropy_context* context = (mbedtls_entropy_context*) NewPtr(sizeof(mbedtls_entropy_context));
    mbedtls_entropy_init(context);
    mbedtls_entropy_add_source(context, mbedtls_hardclock_poll, NULL,
                                sizeof(unsigned long),
                                MBEDTLS_ENTROPY_SOURCE_WEAK );
    return DoAllocateCObjectBinary(context, EntropyDtor);
}

// ------------------------------------------------------------------------ //
// * EntropyDtor( void* )
// ------------------------------------------------------------------------ //
void
PMbedTLS::EntropyDtor( void* ptr )
{
    mbedtls_entropy_free((mbedtls_entropy_context*) ptr);
    ::DisposePtr((Ptr) ptr);
}

// ------------------------------------------------------------------------ //
// * CtrDrbgNew()
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::CtrDrbgNew()
{
    mbedtls_ctr_drbg_context* context = (mbedtls_ctr_drbg_context*) NewPtr(sizeof(mbedtls_ctr_drbg_context));
    mbedtls_ctr_drbg_init(context);
    return DoAllocateCObjectBinary(context, CtrDrbgDtor);
}

// ------------------------------------------------------------------------ //
// * CtrDrbgDtor( void* )
// ------------------------------------------------------------------------ //
void
PMbedTLS::CtrDrbgDtor( void* ptr )
{
    mbedtls_ctr_drbg_free((mbedtls_ctr_drbg_context*) ptr);
    ::DisposePtr((Ptr) ptr);
}

// ------------------------------------------------------------------------ //
// * CtrDrbgSeed( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::CtrDrbgSeed(RefArg inCtrDrbgContextBin, RefArg inEntropyContextBin, RefArg inPersBin)
{
    char buffer[512];
    int result;
    WITH_COBJECT_PTR(inCtrDrbgContextBin, mbedtls_ctr_drbg_context, ctr_drbg)
    WITH_COBJECT_PTR(inEntropyContextBin, mbedtls_entropy_context, entropy)
    WITH_LOCKED_BINARY_MAYBE_STRING(inPersBin, inPersBinPtr, persLen)
        result = mbedtls_ctr_drbg_seed(ctr_drbg, mbedtls_entropy_func, entropy, inPersBinPtr, persLen);
    END_WITH_LOCKED_BINARY_MAYBE_STRING(inPersBin, inPersBinPtr)
    END_WITH_COBJECT_PTR(inEntropyContextBin)
    END_WITH_COBJECT_PTR(inCtrDrbgContextBin)

    return MAKEINT(result);
}

// ------------------------------------------------------------------------ //
// * PKNew()
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::PKNew()
{
    mbedtls_pk_context* context = (mbedtls_pk_context*) NewPtr(sizeof(mbedtls_pk_context));
    mbedtls_pk_init(context);
    return DoAllocateCObjectBinary(context, PKDtor);
}

// ------------------------------------------------------------------------ //
// * PKDtor( void* )
// ------------------------------------------------------------------------ //
void
PMbedTLS::PKDtor( void* ptr )
{
    mbedtls_pk_free((mbedtls_pk_context*) ptr);
    ::DisposePtr((Ptr) ptr);
}

// ------------------------------------------------------------------------ //
// * SSLNew()
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLNew()
{
    SSLContextWithCallbacks* st = new SSLContextWithCallbacks();
    mbedtls_ssl_init(&st->context);
    return DoAllocateCObjectBinary(st, SSLDtor);
}

// ------------------------------------------------------------------------ //
// * SSLDtor()
// ------------------------------------------------------------------------ //
void
PMbedTLS::SSLDtor(void* ptr)
{
    SSLContextWithCallbacks* st = (SSLContextWithCallbacks*) ptr;
    mbedtls_ssl_free(&st->context);
    delete st;
}

// ------------------------------------------------------------------------ //
// * SSLCheckPending(RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLCheckPending(RefArg inSSLBin)
{
    int ret;
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        ret = mbedtls_ssl_check_pending(context);
    END_WITH_COBJECT_PTR(inSSLBin)
    return MAKEINT(ret);
}

// ------------------------------------------------------------------------ //
// * SSLCloseNotify(RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLCloseNotify(RefArg inSSLBin)
{
    int ret;
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        ret = mbedtls_ssl_close_notify(context);
    END_WITH_COBJECT_PTR(inSSLBin)
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * SSLGetALPNProtocol(RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLGetALPNProtocol(RefArg inSSLBin)
{
    const char* ret;
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        ret = mbedtls_ssl_get_alpn_protocol(context);
    END_WITH_COBJECT_PTR(inSSLBin)
    if (ret == NULL) {
        return NILREF;
    }
    return ::MakeString(ret);
}

// ------------------------------------------------------------------------ //
// * SSLGetBytesAvail(RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLGetBytesAvail(RefArg inSSLBin)
{
    int ret;
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        ret = mbedtls_ssl_get_bytes_avail(context);
    END_WITH_COBJECT_PTR(inSSLBin)
    return MAKEINT(ret);
}

// ------------------------------------------------------------------------ //
// * SSLGetCiphersuite(RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLGetCiphersuite(RefArg inSSLBin)
{
    const char* ret;
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        ret = mbedtls_ssl_get_ciphersuite(context);
    END_WITH_COBJECT_PTR(inSSLBin)
    if (ret == NULL) {
        return NILREF;
    }
    return ::MakeString(ret);
}

// ------------------------------------------------------------------------ //
// * SSLGetMaxFragLen(RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLGetMaxFragLen(RefArg inSSLBin)
{
    int ret;
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        ret = mbedtls_ssl_get_max_frag_len(context);
    END_WITH_COBJECT_PTR(inSSLBin)
    return MAKEINT(ret);
}

// ------------------------------------------------------------------------ //
// * SSLGetMaxOutRecordPayload(RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLGetMaxOutRecordPayload(RefArg inSSLBin)
{
    int ret;
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        ret = mbedtls_ssl_get_max_frag_len(context);
    END_WITH_COBJECT_PTR(inSSLBin)
    if (ret >= 0) return MAKEINT(ret);
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * SSLGetPeerCert(RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLGetPeerCert(RefArg inSSLBin)
{
    const mbedtls_x509_crt* ret;
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        ret = mbedtls_ssl_get_peer_cert(context);
    END_WITH_COBJECT_PTR(inSSLBin)
    if (ret == NULL) {
        return NILREF;
    }
    return DoAllocateCObjectBinary((void*) ret, NOPDtor);
}

// ------------------------------------------------------------------------ //
// * SSLGetRecordExpansion(RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLGetRecordExpansion(RefArg inSSLBin)
{
    int ret;
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        ret = mbedtls_ssl_get_record_expansion(context);
    END_WITH_COBJECT_PTR(inSSLBin)
    if (ret >= 0) return MAKEINT(ret);
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * SSLGetSession(RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLGetSession(RefArg inSSLBin)
{
    int ret;
    mbedtls_ssl_session* session = (mbedtls_ssl_session*) ::NewPtr(sizeof(mbedtls_ssl_session));
    mbedtls_ssl_session_init(session);
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        ret = mbedtls_ssl_get_session(context, session);
    END_WITH_COBJECT_PTR(inSSLBin)
    if (ret == 0) {
        return DoAllocateCObjectBinary(session, SSLSessionDtor);
    }
    SSLSessionDtor(session);
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * SSLGetVerifyResult(RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLGetVerifyResult(RefArg inSSLBin)
{
    uint32_t ret;
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        ret = mbedtls_ssl_get_verify_result(context);
    END_WITH_COBJECT_PTR(inSSLBin)
    if (ret == -1u) {
        return NILREF;
    }
    return MAKEINT(ret);
}

// ------------------------------------------------------------------------ //
// * SSLGetVersion(RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLGetVersion(RefArg inSSLBin)
{
    const char* ret;
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        ret = mbedtls_ssl_get_version(context);
    END_WITH_COBJECT_PTR(inSSLBin)
    if (ret == NULL) {
        return NILREF;
    }
    return ::MakeString(ret);
}

// ------------------------------------------------------------------------ //
// * SSLHandshake(RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLHandshake(RefArg inSSLBin)
{
    int ret;
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        ret = mbedtls_ssl_handshake(context);
    END_WITH_COBJECT_PTR(inSSLBin)
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * SSLHandshakeStep(RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLHandshakeStep(RefArg inSSLBin)
{
    int ret;
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        ret = mbedtls_ssl_handshake_step(context);
    END_WITH_COBJECT_PTR(inSSLBin)
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * SSLRead(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLRead(RefArg inSSLBin, RefArg inLen)
{
    int len = RINT(inLen);
    RefVar result = AllocateBinary(SYM(bytes), len);
    int ret;
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        WITH_LOCKED_BINARY(result, buf)
            ret = mbedtls_ssl_read(context, (unsigned char*) buf, len);
        END_WITH_LOCKED_BINARY(result)
    END_WITH_COBJECT_PTR(inSSLBin)
    if (ret == 0) return SYM(eof);
    if (ret < 0) return CastOrThrowError(ret);
    SetLength(result, ret);
    return result;
}

// ------------------------------------------------------------------------ //
// * SSLRenegotiate(RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLRenegotiate(RefArg inSSLBin)
{
    int ret;
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        ret = mbedtls_ssl_renegotiate(context);
    END_WITH_COBJECT_PTR(inSSLBin)
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * SSLSendAlertMessage(RefArg, RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLSendAlertMessage(RefArg inSSLBin, RefArg inLevel, RefArg inMessage)
{
    int ret;
    unsigned char level;
    unsigned char message;
    if (ISINT(inLevel)) {
        level = RINT(inLevel);
    } else if (EQ(inLevel, SYM(warning))) {
        level = MBEDTLS_SSL_ALERT_LEVEL_WARNING;
    } else if (EQ(inLevel, SYM(fatal))) {
        level = MBEDTLS_SSL_ALERT_LEVEL_FATAL;
    } else {
        ThrowBadTypeWithFrameData(kNSErrBadArgs, inLevel);
    }
    if (ISINT(inMessage)) {
        message = RINT(inMessage);
    } else {
        ThrowBadTypeWithFrameData(kNSErrNotAnInteger, inMessage);
    }
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        ret = mbedtls_ssl_send_alert_message(context, inLevel, inMessage);
    END_WITH_COBJECT_PTR(inSSLBin)
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * SSLSessionReset(RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLSessionReset(RefArg inSSLBin)
{
    int ret;
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        ret = mbedtls_ssl_session_reset(context);
    END_WITH_COBJECT_PTR(inSSLBin)
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * SSLSendCallback(void *ctx, const unsigned char *buf, size_t len)
// ------------------------------------------------------------------------ //
int
PMbedTLS::SSLSendCallback(void *ctx, const unsigned char *buf, size_t len)
{
    SSLContextWithCallbacks* context = (SSLContextWithCallbacks*) ctx;
    RefVar buffer = AllocateBinary(SYM(bytes), len);
    WITH_LOCKED_BINARY(buffer, bufferPtr)
        ::BlockMove(buf, bufferPtr, len);
    END_WITH_LOCKED_BINARY(buffer)
    RefVar result = NSCall(context->sendFunction, buffer);
    if (IsSymbol(result)) {
        if (EQ(result, SYM(want_write))) {
            return MBEDTLS_ERR_SSL_WANT_WRITE;
        }
    }
    if (!ISINT(result)) {
        ThrowBadTypeWithFrameData(kNSErrNotAnInteger, result);
    }
    return RINT(result);
}

// ------------------------------------------------------------------------ //
// * SSLRecvCallback(void *ctx, unsigned char *buf, size_t len)
// ------------------------------------------------------------------------ //
int
PMbedTLS::SSLRecvCallback(void *ctx, unsigned char *buf, size_t len)
{
    SSLContextWithCallbacks* context = (SSLContextWithCallbacks*) ctx;
    RefVar result = NSCall(context->recvFunction, MAKEINT(len));
    if (IsSymbol(result)) {
        if (EQ(result, SYM(want_read))) {
            return MBEDTLS_ERR_SSL_WANT_READ;
        }
    }
    if (IsBinary(result)) {
        int count = Length(result);
        WITH_LOCKED_BINARY(result, resultPtr)
            ::BlockMove((const void*) resultPtr, buf, count);
        END_WITH_LOCKED_BINARY(result)
        return count;
    }
    if (!ISINT(result)) {
        ThrowBadTypeWithFrameData(kNSErrNotAnInteger, result);
    }
    return RINT(result);
}

// ------------------------------------------------------------------------ //
// * SSLSetBIO(RefArg, RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLSetBIO(RefArg inSSLBin, RefArg inSendCb, RefArg inRecvCb)
{
    WITH_COBJECT_PTR(inSSLBin, SSLContextWithCallbacks, ctx)
        ctx->sendFunction = inSendCb;
        ctx->recvFunction = inRecvCb;
        mbedtls_ssl_set_bio(&ctx->context, ctx, PMbedTLS::SSLSendCallback, PMbedTLS::SSLRecvCallback, NULL);
    END_WITH_COBJECT_PTR(inSSLBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLSetClientTransportID(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLSetClientTransportID(RefArg inSSLBin, RefArg inTransportLevelInfo)
{
    int ret;
    long inTransportLevelInfoLen = Length(inTransportLevelInfo);
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        WITH_LOCKED_BINARY_MAYBE_STRING(inTransportLevelInfo, inTransportLevelInfoPtr, inTransportLevelInfoLen)
            ret = mbedtls_ssl_set_client_transport_id(context, inTransportLevelInfoPtr, inTransportLevelInfoLen);
        END_WITH_LOCKED_BINARY_MAYBE_STRING(inTransportLevelInfo, inTransportLevelInfoPtr)
    END_WITH_COBJECT_PTR(inSSLBin)
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * SSLSetDatagramPacking(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLSetDatagramPacking(RefArg inSSLBin, RefArg inAllowPacking)
{
    unsigned allow_packing = !ISNIL(inAllowPacking);
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        mbedtls_ssl_set_datagram_packing(context, allow_packing);
    END_WITH_COBJECT_PTR(inSSLBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLSetHostname(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLSetHostname(RefArg inSSLBin, RefArg inHostname)
{
    int ret;
    char* hostname;
    char hostnameBuf[MBEDTLS_SSL_MAX_HOST_NAME_LEN + 1];
    if (IsBinary(inHostname)) {
        WITH_LOCKED_BINARY_MAYBE_STRING(inHostname, inHostnamePtr, len)
            ::BlockMove((const void*) inHostnamePtr, hostnameBuf, len);
            hostnameBuf[len] = 0;
        END_WITH_LOCKED_BINARY_MAYBE_STRING(inHostname, inHostnamePtr)
        hostname = hostnameBuf;
    } else if (ISNIL(inHostname)) {
        hostname = NULL;
    } else {
        ThrowBadTypeWithFrameData(kNSErrNotAString, inHostname);
    }
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        ret = mbedtls_ssl_set_hostname(context, hostname);
    END_WITH_COBJECT_PTR(inSSLBin)
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * SSLSetHsAuthMode(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLSetHsAuthMode(RefArg inSSLBin, RefArg inAuthmode)
{
    int authMode = CastAuthMode(inAuthmode);
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        mbedtls_ssl_set_hs_authmode(context, authMode);
    END_WITH_COBJECT_PTR(inSSLBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLSetHsCAChain(RefArg, RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLSetHsCAChain(RefArg inSSLBin, RefArg inX509CrtBin, RefArg inX509CrlBin)
{
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        WITH_COBJECT_PTR_OPT(inX509CrtBin, mbedtls_x509_crt, crt)
            WITH_COBJECT_PTR_OPT(inX509CrlBin, mbedtls_x509_crl, crl)
                mbedtls_ssl_set_hs_ca_chain(context, crt, crl);
            END_WITH_COBJECT_PTR_OPT(inX509CrlBin)
        END_WITH_COBJECT_PTR_OPT(inX509CrtBin)
    END_WITH_COBJECT_PTR(inSSLBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLSetHsOwnCert(RefArg, RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLSetHsOwnCert(RefArg inSSLBin, RefArg inX509CrtBin, RefArg inPKBin)
{
    int ret;
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        WITH_COBJECT_PTR(inX509CrtBin, mbedtls_x509_crt, crt)
            WITH_COBJECT_PTR(inPKBin, mbedtls_pk_context, pk)
                ret = mbedtls_ssl_set_hs_own_cert(context, crt, pk);
            END_WITH_COBJECT_PTR(inPKBin)
        END_WITH_COBJECT_PTR(inX509CrtBin)
    END_WITH_COBJECT_PTR(inSSLBin)
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * SSLSetHsPSK(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLSetHsPSK(RefArg inSSLBin, RefArg inPSKBin)
{
    int ret;
    if (!IsBinary(inPSKBin)) {
        ThrowBadTypeWithFrameData(kNSErrNotABinaryObject, inPSKBin);
    }
    long len = Length(inPSKBin);
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        WITH_LOCKED_BINARY(inPSKBin, inPSKBinPtr)
            ret = mbedtls_ssl_set_hs_psk(context, (const unsigned char*) inPSKBinPtr, len);
        END_WITH_LOCKED_BINARY(inPSKBin)
    END_WITH_COBJECT_PTR(inSSLBin)
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * SSLSetMTU(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLSetMTU(RefArg inSSLBin, RefArg inMTU)
{
    if (!ISINT(inMTU)) ThrowBadTypeWithFrameData(kNSErrNotAnInteger, inMTU);
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        mbedtls_ssl_set_mtu(context, RINT(inMTU));
    END_WITH_COBJECT_PTR(inSSLBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLSetSession(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLSetSession(RefArg inSSLBin, RefArg inSession)
{
    int ret;
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_session, session)
            ret = mbedtls_ssl_set_session(context, session);
        END_WITH_COBJECT_PTR(inSSLBin)
    END_WITH_COBJECT_PTR(inSSLBin)
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * SSLSetTimerCallback(void *ctx, uint32_t int_ms, uint32_t fin_ms)
// ------------------------------------------------------------------------ //
void
PMbedTLS::SSLSetTimerCallback(void *ctx, uint32_t int_ms, uint32_t fin_ms)
{
    SSLContextWithCallbacks* context = (SSLContextWithCallbacks*) ctx;
    NSCall(context->setTimerFunction, MAKEINT(int_ms), MAKEINT(fin_ms));
}

// ------------------------------------------------------------------------ //
// * SSLGetTimerCallback(void *ctx)
// ------------------------------------------------------------------------ //
int
PMbedTLS::SSLGetTimerCallback(void *ctx)
{
    SSLContextWithCallbacks* context = (SSLContextWithCallbacks*) ctx;
    RefVar result = NSCall(context->getTimerFunction);
    return RINT(result);
}

// ------------------------------------------------------------------------ //
// * SSLSetTimerCb(RefArg, RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLSetTimerCb(RefArg inSSLBin, RefArg inSetTimerCb, RefArg inGetTimerCb)
{
    WITH_COBJECT_PTR(inSSLBin, SSLContextWithCallbacks, ctx)
        ctx->setTimerFunction = inSetTimerCb;
        ctx->getTimerFunction = inGetTimerCb;
        mbedtls_ssl_set_timer_cb(&ctx->context, ctx, SSLSetTimerCallback, SSLGetTimerCallback);
    END_WITH_COBJECT_PTR(inSSLBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLSetup(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLSetup(RefArg inSSLBin, RefArg inSSLConfigBin)
{
    int ret;
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
            ret = mbedtls_ssl_setup(context, config);
        END_WITH_COBJECT_PTR(inSSLConfigBin)
    END_WITH_COBJECT_PTR(inSSLBin)
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * SSLWrite(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLWrite(RefArg inSSLBin, RefArg inData)
{
    int ret;
    if (!IsBinary(inData)) {
        ThrowBadTypeWithFrameData(kNSErrNotABinaryObject, inData);
    }
    long len = Length(inData);
    WITH_COBJECT_PTR(inSSLBin, mbedtls_ssl_context, context)
        WITH_LOCKED_BINARY(inData, inDataPtr)
            ret = mbedtls_ssl_write(context, (const unsigned char*) inDataPtr, len);
        END_WITH_LOCKED_BINARY(inData)
    END_WITH_COBJECT_PTR(inSSLBin)
    if (ret >= 0) return MAKEINT(ret);
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * SSLCacheNew()
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLCacheNew()
{
    mbedtls_ssl_cache_context* cache = (mbedtls_ssl_cache_context*) NewPtr(sizeof(mbedtls_ssl_cache_context));
    mbedtls_ssl_cache_init(cache);
    return DoAllocateCObjectBinary(cache, SSLCacheDtor);
}

// ------------------------------------------------------------------------ //
// * SSLCacheDtor( void* )
// ------------------------------------------------------------------------ //
void
PMbedTLS::SSLCacheDtor( void* ptr )
{
    mbedtls_ssl_cache_free((mbedtls_ssl_cache_context*) ptr);
    ::DisposePtr((Ptr) ptr);
}

// ------------------------------------------------------------------------ //
// * SSLCacheSetMaxEntries(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLCacheSetMaxEntries(RefArg inSSLCacheBin, RefArg inMaxEntries)
{
    if (!ISINT(inMaxEntries)) ThrowBadTypeWithFrameData(kNSErrNotAnInteger, inMaxEntries);
    WITH_COBJECT_PTR(inSSLCacheBin, mbedtls_ssl_cache_context, cache)
        mbedtls_ssl_cache_set_max_entries(cache, RINT(inMaxEntries));
    END_WITH_COBJECT_PTR(inSSLCacheBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLCacheSetTimeout(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLCacheSetTimeout(RefArg inSSLCacheBin, RefArg inTimeout)
{
    if (!ISINT(inTimeout)) ThrowBadTypeWithFrameData(kNSErrNotAnInteger, inTimeout);
    WITH_COBJECT_PTR(inSSLCacheBin, mbedtls_ssl_cache_context, cache)
        mbedtls_ssl_cache_set_timeout(cache, RINT(inTimeout));
    END_WITH_COBJECT_PTR(inSSLCacheBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfigNew()
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfigNew()
{
    SSLConfigWithCallbacks* st = new SSLConfigWithCallbacks();
    st->obj = this;
    mbedtls_ssl_config_init(&st->config);
    return DoAllocateCObjectBinary(st, SSLConfigDtor);
}

// ------------------------------------------------------------------------ //
// * SSLConfigDtor( void* )
// ------------------------------------------------------------------------ //
void
PMbedTLS::SSLConfigDtor( void* ptr )
{
    SSLConfigWithCallbacks* st = (SSLConfigWithCallbacks*) ptr;
    mbedtls_ssl_config_free(&st->config);
    delete st;
}

// ------------------------------------------------------------------------ //
// * SSLConfALPNProtocol(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfALPNProtocol(RefArg inSSLConfigBin, RefArg inProtos)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfARC4Support(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfARC4Support(RefArg inSSLConfigBin, RefArg inARC4)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfAuthMode( RefArg, RefArg )
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfAuthMode( RefArg inSSLConfigBin, RefArg inAuthMode )
{
    int authMode = CastAuthMode(inAuthMode);
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        mbedtls_ssl_conf_authmode(config, authMode);
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfCAChain( RefArg, RefArg )
// ------------------------------------------------------------------------ //
// Configure CA chain.
Ref
PMbedTLS::SSLConfCAChain( RefArg inSSLConfigBin, RefArg inX509CrtBin, RefArg inX509CrlBin )
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        WITH_COBJECT_PTR_OPT(inX509CrlBin, mbedtls_x509_crl, crl)
            WITH_COBJECT_PTR_OPT(inX509CrtBin, mbedtls_x509_crt, crt)
                mbedtls_ssl_conf_ca_chain(config, crt, crl);
            END_WITH_COBJECT_PTR_OPT(inX509CrtBin)
        END_WITH_COBJECT_PTR_OPT(inX509CrlBin)
    END_WITH_COBJECT_PTR(inSSLConfigBin)

    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfCbcRecordSplitting(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfCbcRecordSplitting(RefArg inSSLConfigBin, RefArg inSplit)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfCertProfile(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfCertProfile(RefArg inSSLConfigBin, RefArg inCrtbin)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfCertReqCAList(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfCertReqCAList(RefArg inSSLConfigBin, RefArg inCertReqCAList)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfCiphersuites(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfCiphersuites(RefArg inSSLConfigBin, RefArg inCiphersuites)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfCiphersuitesForVersion(RefArg, RefArg, RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfCiphersuitesForVersion(RefArg inSSLConfigBin, RefArg inCiphersuites, RefArg inMajor, RefArg inMinor)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfCurves(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfCurves(RefArg inSSLConfigBin, RefArg inCurves)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfDbg(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfDbg(RefArg inSSLConfigBin, RefArg inDebugCb)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfDHParamBin(RefArg, RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfDHParamBin(RefArg inSSLConfigBin, RefArg inDHMP, RefArg inDHMG)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfDHParamCtx(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfDHParamCtx(RefArg inSSLConfigBin, RefArg inDHMContextBin)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfDHMMinBitlen(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfDHMMinBitlen(RefArg inSSLConfigBin, RefArg inBitlen)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfDTLSAntiReplay(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfDTLSAntiReplay(RefArg inSSLConfigBin, RefArg inMode)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfDTLSBadMACLimit(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfDTLSBadMACLimit(RefArg inSSLConfigBin, RefArg inLimit)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfDTLSCookies(RefArg, RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfDTLSCookies(RefArg inSSLConfigBin, RefArg inCookiesWriteCb, RefArg inCookiesCheckCb)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfEncryptThenMac(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfEncryptThenMac(RefArg inSSLConfigBin, RefArg inETM)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfEndpoint(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfEndpoint(RefArg inSSLConfigBin, RefArg inEndpoint)
{
    int endpoint = CastEndpoint(inEndpoint);
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        mbedtls_ssl_conf_endpoint(config, endpoint);
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfExportKeysCb(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfExportKeysCb(RefArg inSSLConfigBin, RefArg inExportKeysCb)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfExtendedMasterSecret(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfExtendedMasterSecret(RefArg inSSLConfigBin, RefArg inEMS)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfFallback(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfFallback(RefArg inSSLConfigBin, RefArg inFallback)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfHandshakeTimeout(RefArg, RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfHandshakeTimeout(RefArg inSSLConfigBin, RefArg inMin, RefArg inMax)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        mbedtls_ssl_conf_handshake_timeout(config, RINT(inMin), RINT(inMax));
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfLegacyRenegotiation(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfLegacyRenegotiation(RefArg inSSLConfigBin, RefArg inAllowLegacy)
{
    int legacy;
    if (!IsSymbol(inAllowLegacy)) {
        ThrowBadTypeWithFrameData(kNSErrNotASymbol, inAllowLegacy);
    } else if (EQ(inAllowLegacy, SYM(no_renegotiation))) {
        legacy = MBEDTLS_SSL_LEGACY_NO_RENEGOTIATION;
    } else if (EQ(inAllowLegacy, SYM(allow_renegotiation))) {
        legacy = MBEDTLS_SSL_LEGACY_ALLOW_RENEGOTIATION;
    } else if (EQ(inAllowLegacy, SYM(break_handshake))) {
        legacy = MBEDTLS_SSL_LEGACY_BREAK_HANDSHAKE;
    } else {
        ThrowBadTypeWithFrameData(kNSErrBadArgs, inAllowLegacy);
    }
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        mbedtls_ssl_conf_legacy_renegotiation(config, legacy);
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfMaxFragLen(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfMaxFragLen(RefArg inSSLConfigBin, RefArg inMaxFragLen)
{
    int mfl_code;
    if (!ISINT(inMaxFragLen)) {
        ThrowBadTypeWithFrameData(kNSErrNotAnInteger, inMaxFragLen);
    }

    int maxFragLen = RINT(inMaxFragLen);
    if (maxFragLen == 512) {
        mfl_code = MBEDTLS_SSL_MAX_FRAG_LEN_512;
    } else if (maxFragLen == 1024) {
        mfl_code = MBEDTLS_SSL_MAX_FRAG_LEN_1024;
    } else if (maxFragLen == 2048) {
        mfl_code = MBEDTLS_SSL_MAX_FRAG_LEN_2048;
    } else if (maxFragLen == 4096) {
        mfl_code = MBEDTLS_SSL_MAX_FRAG_LEN_4096;
    } else {
        ThrowBadTypeWithFrameData(kNSErrBadArgs, inMaxFragLen);
    }
    int ret;
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ret = mbedtls_ssl_conf_max_frag_len(config, mfl_code);
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * SSLConfMaxVersion(RefArg, RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfMaxVersion(RefArg inSSLConfigBin, RefArg inMajor, RefArg inMinor)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        mbedtls_ssl_conf_max_version(config, RINT(inMajor), RINT(inMinor));
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfMinVersion(RefArg, RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfMinVersion(RefArg inSSLConfigBin, RefArg inMajor, RefArg inMinor)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        mbedtls_ssl_conf_min_version(config, RINT(inMajor), RINT(inMinor));
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfOwnCert(RefArg, RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfOwnCert(RefArg inSSLConfigBin, RefArg inCrtBin, RefArg inPKBin)
{
    int ret;
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        WITH_COBJECT_PTR(inCrtBin, mbedtls_x509_crt, crt)
            WITH_COBJECT_PTR(inPKBin, mbedtls_pk_context, pk)
                ret = mbedtls_ssl_conf_own_cert(config, crt, pk);
            END_WITH_COBJECT_PTR(inPKBin)
        END_WITH_COBJECT_PTR(inCrtBin)
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * SSLConfPSK(RefArg, RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfPSK(RefArg inSSLConfigBin, RefArg inPSK, RefArg inPSKIdentity)
{
    int ret;
    long pskLen = Length(inPSK);
    long pskIdentityLen = Length(inPSKIdentity);
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        WITH_LOCKED_BINARY(inPSK, inPSKPtr)
            WITH_LOCKED_BINARY(inPSKIdentity, inPSKIdentityPtr)
                ret = mbedtls_ssl_conf_psk(config, (const unsigned char*) inPSKPtr, pskLen, (const unsigned char*) inPSKIdentityPtr, pskIdentityLen);
            END_WITH_LOCKED_BINARY(inPSKIdentity)
        END_WITH_LOCKED_BINARY(inPSK)
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * SSLConfPSKCb(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfPSKCb(RefArg inSSLConfigBin, RefArg inPSKCb)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfRenegotiation(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfRenegotiation(RefArg inSSLConfigBin, RefArg inRenegotiation)
{
    int renegotiation = MBEDTLS_SSL_RENEGOTIATION_DISABLED;
    if (EQ(inRenegotiation, SYM(enabled))) {
        renegotiation = MBEDTLS_SSL_RENEGOTIATION_ENABLED;
    } else if (EQ(inRenegotiation, SYM(disabled))) {
        renegotiation = MBEDTLS_SSL_RENEGOTIATION_DISABLED;
    } else if (ISFALSE(inRenegotiation)) {
        renegotiation = MBEDTLS_SSL_RENEGOTIATION_DISABLED;
    } else if (ISTRUE(inRenegotiation)) {
        renegotiation = MBEDTLS_SSL_RENEGOTIATION_ENABLED;
    } else {
        ThrowBadTypeWithFrameData(kNSErrNotTrueOrNil, inRenegotiation);
    }
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        mbedtls_ssl_conf_renegotiation(config, renegotiation);
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfRenegotiationEnforced(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfRenegotiationEnforced(RefArg inSSLConfigBin, RefArg inMaxRecords)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        mbedtls_ssl_conf_renegotiation_enforced(config, RINT(inMaxRecords));
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfRenegotiationPeriod(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfRenegotiationPeriod(RefArg inSSLConfigBin, RefArg inPeriod)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfRng( RefArg, RefArg )
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfRng( RefArg inSSLConfigBin, RefArg inCtrDrbgContextBin )
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        RefVar ctrDrbgBin;
        WITH_COBJECT_PTR(ctrDrbgBin, mbedtls_ctr_drbg_context, ctr_drbg)
            mbedtls_ssl_conf_rng(config, mbedtls_ctr_drbg_random, ctr_drbg);
        END_WITH_COBJECT_PTR(ctrDrbgBin)
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfSessionCache(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfSessionCache(RefArg inSSLConfigBin, RefArg inCacheBin)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfSessionTickets(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfSessionTickets(RefArg inSSLConfigBin, RefArg inUseTickets)
{
    int use_tickets = MBEDTLS_SSL_SESSION_TICKETS_DISABLED;
    if (EQ(inUseTickets, SYM(enabled))) {
        use_tickets = MBEDTLS_SSL_SESSION_TICKETS_ENABLED;
    } else if (EQ(inUseTickets, SYM(disabled))) {
        use_tickets = MBEDTLS_SSL_SESSION_TICKETS_DISABLED;
    } else if (ISFALSE(inUseTickets)) {
        use_tickets = MBEDTLS_SSL_SESSION_TICKETS_DISABLED;
    } else if (ISTRUE(inUseTickets)) {
        use_tickets = MBEDTLS_SSL_SESSION_TICKETS_ENABLED;
    } else {
        ThrowBadTypeWithFrameData(kNSErrNotTrueOrNil, inUseTickets);
    }
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        mbedtls_ssl_conf_session_tickets(config, use_tickets);
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfSessionTicketsCb(RefArg, RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfSessionTicketsCb(RefArg inSSLConfigBin, RefArg inTicketWriteCb, RefArg inTicketParseCb)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfSigHashes(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfSigHashes(RefArg inSSLConfigBin, RefArg inSigHashes)
{
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ThrowMsg("Unimplemented");
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfSNICallback(void*, mbedtls_x509_crt*, int, uint32_t*)
// ------------------------------------------------------------------------ //
int
PMbedTLS::SSLConfSNICallback(void *ctx, mbedtls_ssl_context *ssl, const unsigned char* hostname, size_t len)
{
    SSLConfigWithCallbacks* context = (SSLConfigWithCallbacks*) ctx;
    RefVar sslBin = context->obj->DoAllocateCObjectBinary((void*) ssl, NOPDtor);
    RefVar hostnameBin = AllocateBinary(SYM(string), (len + 1) * 2);
    WITH_LOCKED_BINARY(hostnameBin, hostnameBinPtr)
        Character* str = (Character*) hostnameBinPtr;
        for (int i = 0; i < len; i++) {
            str[i] = hostname[i];
        }
        str[len] = 0;
    END_WITH_LOCKED_BINARY(hostnameBin)
    RefVar result = NSCall(context->sniFunction, sslBin, hostnameBin);
    return RINT(result);
}

// ------------------------------------------------------------------------ //
// * SSLConfSNI(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfSNI(RefArg inSSLConfigBin, RefArg inSNICb)
{
    WITH_COBJECT_PTR(inSSLConfigBin, SSLConfigWithCallbacks, cfg)
        cfg->sniFunction = inSNICb;
        mbedtls_ssl_conf_sni(&cfg->config, SSLConfSNICallback, cfg);
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfTransport(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfTransport(RefArg inSSLConfigBin, RefArg inTransport)
{
    int transport = CastTransport(inTransport);
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        mbedtls_ssl_conf_transport(config, transport);
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfTruncatedHMAC(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfTruncatedHMAC(RefArg inSSLConfigBin, RefArg inTruncate)
{
    int truncate = MBEDTLS_SSL_TRUNC_HMAC_DISABLED;
    if (EQ(inTruncate, SYM(enabled))) {
        truncate = MBEDTLS_SSL_TRUNC_HMAC_ENABLED;
    } else if (EQ(inTruncate, SYM(disabled))) {
        truncate = MBEDTLS_SSL_TRUNC_HMAC_DISABLED;
    } else if (ISFALSE(inTruncate)) {
        truncate = MBEDTLS_SSL_TRUNC_HMAC_DISABLED;
    } else if (ISTRUE(inTruncate)) {
        truncate = MBEDTLS_SSL_TRUNC_HMAC_ENABLED;
    } else {
        ThrowBadTypeWithFrameData(kNSErrNotTrueOrNil, inTruncate);
    }
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        mbedtls_ssl_conf_truncated_hmac(config, truncate);
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfVerifyCallback(void*, mbedtls_x509_crt*, int, uint32_t*)
// ------------------------------------------------------------------------ //
int
PMbedTLS::SSLConfVerifyCallback(void *ctx, mbedtls_x509_crt *crt, int certificate_depth, uint32_t *flags)
{
    SSLConfigWithCallbacks* context = (SSLConfigWithCallbacks*) ctx;
    RefVar certificate = context->obj->DoAllocateCObjectBinary((void*) crt, NOPDtor);
    RefVar newFlags = NSCall(context->verifyFunction, certificate, MAKEINT(certificate_depth), MAKEINT(*flags));
    if (EQ(newFlags, SYM(fatal))) return MBEDTLS_ERR_X509_FATAL_ERROR;
    *flags = RINT(newFlags);
    return 0;
}

// ------------------------------------------------------------------------ //
// * SSLConfVerify(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfVerify(RefArg inSSLConfigBin, RefArg inVerifyCb)
{
    WITH_COBJECT_PTR(inSSLConfigBin, SSLConfigWithCallbacks, cfg)
        cfg->verifyFunction = inVerifyCb;
        mbedtls_ssl_conf_verify(&cfg->config, SSLConfVerifyCallback, cfg);
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * SSLConfigDefaults( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLConfigDefaults( RefArg inSSLConfigBin, RefArg inEndpoint, RefArg inTransport )
{
    int ret;
    int endpoint = CastEndpoint(inEndpoint);
    int transport = CastTransport(inTransport);
    WITH_COBJECT_PTR(inSSLConfigBin, mbedtls_ssl_config, config)
        ret = mbedtls_ssl_config_defaults(config, endpoint, transport, MBEDTLS_SSL_PRESET_DEFAULT);
    END_WITH_COBJECT_PTR(inSSLConfigBin)
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * SSLSessionNew()
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::SSLSessionNew()
{
    mbedtls_ssl_session* session = (mbedtls_ssl_session*) NewPtr(sizeof(mbedtls_ssl_session));
    mbedtls_ssl_session_init(session);
    return DoAllocateCObjectBinary(session, SSLSessionDtor);
}

// ------------------------------------------------------------------------ //
// * SSLSessionDtor( void* )
// ------------------------------------------------------------------------ //
void
PMbedTLS::SSLSessionDtor( void* ptr )
{
    mbedtls_ssl_session_free((mbedtls_ssl_session*) ptr);
    ::DisposePtr((Ptr) ptr);
}

// ------------------------------------------------------------------------ //
// * X509CrtNew()
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::X509CrtNew()
{
    mbedtls_x509_crt* crt = (mbedtls_x509_crt*) NewPtr(sizeof(mbedtls_x509_crt));
    mbedtls_x509_crt_init(crt);
    return DoAllocateCObjectBinary(crt, X509CrtDtor);
}

// ------------------------------------------------------------------------ //
// * X509CrtDtor( void* )
// ------------------------------------------------------------------------ //
void
PMbedTLS::X509CrtDtor( void* ptr )
{
    mbedtls_x509_crt_free((mbedtls_x509_crt*) ptr);
    ::DisposePtr((Ptr) ptr);
}

// ------------------------------------------------------------------------ //
// * X509CrtParse( RefArg, RefArg )
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::X509CrtParse( RefArg inX509CrtBin, RefArg inCertificate )
{
    int ret;
    WITH_COBJECT_PTR(inX509CrtBin, mbedtls_x509_crt, crt)
        if (IsBinary(inCertificate)) {
            WITH_LOCKED_BINARY_MAYBE_STRING(inCertificate, inCertificateBinPtr, inCertificateBinPtrLen)
                ret = mbedtls_x509_crt_parse(crt, inCertificateBinPtr, inCertificateBinPtrLen);
            END_WITH_LOCKED_BINARY_MAYBE_STRING(inCertificate, inCertificateBinPtr)
        } else if (IsArray(inCertificate)) {
            ULong len = Length(inCertificate);
            unsigned char* certificate = (unsigned char*) ::NewPtr(len);
            for (ULong i = 0; i < len; i++) {
                certificate[i] = (unsigned char) RINT(GetArraySlot(inCertificate, i));
            }
            ret = mbedtls_x509_crt_parse(crt, certificate, len);
        } else {
            ThrowBadTypeWithFrameData(kNSErrNotAnArrayOrString, inCertificate);
        }
    END_WITH_COBJECT_PTR(inX509CrtBin)
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * X509CrtCount(RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::X509CrtCount(RefArg inX509CrtBin)
{
    int ret = 0;
    WITH_COBJECT_PTR(inX509CrtBin, mbedtls_x509_crt, crt)
        if (crt->raw.p != NULL) {
            ret = 1;
            while (crt->next != NULL) {
                crt = crt->next;
                ret++;
            }
        }
    END_WITH_COBJECT_PTR(inX509CrtBin)
    return MAKEINT(ret);
}

// ------------------------------------------------------------------------ //
// * X509CrtGetDER(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::X509CrtGetDER(RefArg inX509CrtBin, RefArg inIndex)
{
    RefVar result;
    WITH_COBJECT_PTR(inX509CrtBin, mbedtls_x509_crt, crt)
        if (crt->raw.p != NULL) {
            result = AllocateBinary(SYM(dem), crt->raw.len);
            WITH_LOCKED_BINARY(result, resultPtr)
                BlockMove(crt->raw.p, resultPtr, crt->raw.len);
            END_WITH_LOCKED_BINARY(result)
        }
    END_WITH_COBJECT_PTR(inX509CrtBin)
    return result;
}

// ------------------------------------------------------------------------ //
// * X509CrtGetVersion(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::X509CrtGetVersion(RefArg inX509CrtBin, RefArg inIndex)
{
    int result;
    long index = RINT(inIndex);
    WITH_COBJECT_PTR(inX509CrtBin, mbedtls_x509_crt, crt)
        if (crt->raw.p == NULL) {
            ThrowBadTypeWithFrameData(kNSErrOutOfBounds, inIndex);
        }
        while (index != 0) {
            crt = crt->next;
            if (crt == NULL) {
                ThrowBadTypeWithFrameData(kNSErrOutOfBounds, inIndex);
            }
            index--;
        }
        result = crt->version;
    END_WITH_COBJECT_PTR(inX509CrtBin)
    return MAKEINT(result);
}

// ------------------------------------------------------------------------ //
// * X509CrtGetSerialNumber(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::X509CrtGetSerialNumber(RefArg inX509CrtBin, RefArg inIndex)
{
    RefVar result;
    long index = RINT(inIndex);
    WITH_COBJECT_PTR(inX509CrtBin, mbedtls_x509_crt, crt)
        if (crt->raw.p == NULL) {
            ThrowBadTypeWithFrameData(kNSErrOutOfBounds, inIndex);
        }
        while (index != 0) {
            crt = crt->next;
            if (crt == NULL) {
                ThrowBadTypeWithFrameData(kNSErrOutOfBounds, inIndex);
            }
            index--;
        }
        char buf[512];
        int r = mbedtls_x509_serial_gets(buf, sizeof(buf), &crt->serial);
        if (r < 0) {
            Throw(exMbedTLSException, (void*) r);
        }
        result = MakeString(buf);
    END_WITH_COBJECT_PTR(inX509CrtBin)
    return result;
}

// ------------------------------------------------------------------------ //
// * X509CrtGetIssuerName(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::X509CrtGetIssuerName(RefArg inX509CrtBin, RefArg inIndex)
{
    RefVar result;
    long index = RINT(inIndex);
    WITH_COBJECT_PTR(inX509CrtBin, mbedtls_x509_crt, crt)
        if (crt->raw.p == NULL) {
            ThrowBadTypeWithFrameData(kNSErrOutOfBounds, inIndex);
        }
        while (index != 0) {
            crt = crt->next;
            if (crt == NULL) {
                ThrowBadTypeWithFrameData(kNSErrOutOfBounds, inIndex);
            }
            index--;
        }
        char buf[512];
        int r = mbedtls_x509_dn_gets(buf, sizeof(buf), &crt->issuer);
        if (r < 0) {
            Throw(exMbedTLSException, (void*) r);
        }
        result = MakeString(buf);
    END_WITH_COBJECT_PTR(inX509CrtBin)
    return result;
}

// ------------------------------------------------------------------------ //
// * X509CrtGetSubjectName(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::X509CrtGetSubjectName(RefArg inX509CrtBin, RefArg inIndex)
{
    RefVar result;
    long index = RINT(inIndex);
    WITH_COBJECT_PTR(inX509CrtBin, mbedtls_x509_crt, crt)
        if (crt->raw.p == NULL) {
            ThrowBadTypeWithFrameData(kNSErrOutOfBounds, inIndex);
        }
        while (index != 0) {
            crt = crt->next;
            if (crt == NULL) {
                ThrowBadTypeWithFrameData(kNSErrOutOfBounds, inIndex);
            }
            index--;
        }
        char buf[512];
        int r = mbedtls_x509_dn_gets(buf, sizeof(buf), &crt->subject);
        if (r < 0) {
            Throw(exMbedTLSException, (void*) r);
        }
        result = MakeString(buf);
    END_WITH_COBJECT_PTR(inX509CrtBin)
    return result;
}

// ------------------------------------------------------------------------ //
// * X509CrtGetIssueDate(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::X509CrtGetIssueDate(RefArg inX509CrtBin, RefArg inIndex)
{
    mbedtls_x509_time result;
    long index = RINT(inIndex);
    WITH_COBJECT_PTR(inX509CrtBin, mbedtls_x509_crt, crt)
        if (crt->raw.p == NULL) {
            ThrowBadTypeWithFrameData(kNSErrOutOfBounds, inIndex);
        }
        while (index != 0) {
            crt = crt->next;
            if (crt == NULL) {
                ThrowBadTypeWithFrameData(kNSErrOutOfBounds, inIndex);
            }
            index--;
        }
        char buf[512];
        int r = mbedtls_x509_dn_gets(buf, sizeof(buf), &crt->subject);
        if (r < 0) {
            Throw(exMbedTLSException, (void*) r);
        }
        result = crt->valid_from;
    END_WITH_COBJECT_PTR(inX509CrtBin)
    long minutes = TimeToLocal1904Minutes(result);
    return MAKEINT(minutes);
}

// ------------------------------------------------------------------------ //
// * X509CrtGetExpirationDate(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::X509CrtGetExpirationDate(RefArg inX509CrtBin, RefArg inIndex)
{
    mbedtls_x509_time result;
    long index = RINT(inIndex);
    WITH_COBJECT_PTR(inX509CrtBin, mbedtls_x509_crt, crt)
        if (crt->raw.p == NULL) {
            ThrowBadTypeWithFrameData(kNSErrOutOfBounds, inIndex);
        }
        while (index != 0) {
            crt = crt->next;
            if (crt == NULL) {
                ThrowBadTypeWithFrameData(kNSErrOutOfBounds, inIndex);
            }
            index--;
        }
        char buf[512];
        int r = mbedtls_x509_dn_gets(buf, sizeof(buf), &crt->subject);
        if (r < 0) {
            Throw(exMbedTLSException, (void*) r);
        }
        result = crt->valid_to;
    END_WITH_COBJECT_PTR(inX509CrtBin)
    long minutes = TimeToLocal1904Minutes(result);
    return MAKEINT(minutes);
}

// ------------------------------------------------------------------------ //
// * X509CrtGetSignatureAlgorithm(RefArg, RefArg)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::X509CrtGetSignatureAlgorithm(RefArg inX509CrtBin, RefArg inIndex)
{
    RefVar result;
    long index = RINT(inIndex);
    WITH_COBJECT_PTR(inX509CrtBin, mbedtls_x509_crt, crt)
        if (crt->raw.p == NULL) {
            ThrowBadTypeWithFrameData(kNSErrOutOfBounds, inIndex);
        }
        while (index != 0) {
            crt = crt->next;
            if (crt == NULL) {
                ThrowBadTypeWithFrameData(kNSErrOutOfBounds, inIndex);
            }
            index--;
        }
        char buf[512];
        int r = mbedtls_x509_sig_alg_gets(buf, sizeof(buf), &crt->sig_oid, crt->sig_pk, crt->sig_md, crt->sig_opts);
        if (r < 0) {
            Throw(exMbedTLSException, (void*) r);
        }
        result = MakeString(buf);
    END_WITH_COBJECT_PTR(inX509CrtBin)
    return result;
}

// ------------------------------------------------------------------------ //
// * X509CrlNew()
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::X509CrlNew()
{
    mbedtls_x509_crl* crl = (mbedtls_x509_crl*) NewPtr(sizeof(mbedtls_x509_crl));
    mbedtls_x509_crl_init(crl);
    return DoAllocateCObjectBinary(crl, X509CrlDtor);
}

// ------------------------------------------------------------------------ //
// * X509CrlDtor( void* )
// ------------------------------------------------------------------------ //
void
PMbedTLS::X509CrlDtor( void* ptr )
{
    mbedtls_x509_crl_free((mbedtls_x509_crl*) ptr);
    ::DisposePtr((Ptr) ptr);
}

// ------------------------------------------------------------------------ //
// * X509CrlParse( RefArg, RefArg )
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::X509CrlParse( RefArg inX509CrlBin, RefArg inCRLBin )
{
    int ret;
    WITH_LOCKED_BINARY_MAYBE_STRING(inCRLBin, inCRLBinPtr, inCRLBinPtrLen)
        WITH_COBJECT_PTR(inX509CrlBin, mbedtls_x509_crt, crt)
            ret = mbedtls_x509_crt_parse(crt, inCRLBinPtr, inCRLBinPtrLen);
        END_WITH_COBJECT_PTR(inX509CrlBin)
    END_WITH_LOCKED_BINARY_MAYBE_STRING(inCRLBin, inCRLBinPtr)
    return CastOrThrowError(ret);
}

// ------------------------------------------------------------------------ //
// * FreeCObjectBinary(RefArg)
// ------------------------------------------------------------------------ //
// Free a CObject binary, calling destructor and resetting pointer to null
// and destructor to DisposePtr, a NOP for NULL.
void
PMbedTLS::FreeCObjectBinary(RefArg inBin)
{
    if (IsBinary(inBin)) {
        unwind_protect {
            inBin.Lock();
            CObject* cobject = &((CObjectBin*) (((long) inBin) - 1))->binary;
            if (cobject->ptr) {
                cobject->destructor(cobject->ptr);
                cobject->destructor = (CObjectBinaryProc) DISPOSE_PTR_PUBLIC_JT;
                cobject->ptr = NULL;
            }
        } on_unwind {
            inBin.Unlock();
        } end_unwind;
    }
}

// ------------------------------------------------------------------------ //
// * DoAllocateCObjectBinary(RefArg)
// ------------------------------------------------------------------------ //
// Allocate a CObject binary and register it to the weak array.
Ref
PMbedTLS::DoAllocateCObjectBinary(void* ptr, CObjectBinaryProc dtor)
{
    RefVar bin = AllocateCObjectBinary(ptr, dtor);
    long len = Length(*mPointers);
    for (int index = 0; index < len; index++) {
        if (mPointers->GetArraySlot(index) == NILREF) {
            mPointers->SetArraySlot(index, bin);
            return bin;
        }
    }
    mPointers->AddArraySlot(bin);
    return bin;
}

// ------------------------------------------------------------------------ //
// * NOPDtor( void* )
// ------------------------------------------------------------------------ //
void
PMbedTLS::NOPDtor( void* ptr )
{
}

// ------------------------------------------------------------------------ //
// * CastOrThrowError(int)
// ------------------------------------------------------------------------ //
Ref
PMbedTLS::CastOrThrowError(int err)
{
    if (err == 0) return SYM(ok);
    if (err == MBEDTLS_ERR_SSL_WANT_READ) return SYM(want_read);
    if (err == MBEDTLS_ERR_SSL_WANT_WRITE) return SYM(want_write);
    if (err == MBEDTLS_ERR_SSL_ASYNC_IN_PROGRESS) return SYM(async_in_progress);
    if (err == MBEDTLS_ERR_SSL_CRYPTO_IN_PROGRESS) return SYM(crypto_in_progress);
    if (err == MBEDTLS_ERR_SSL_HELLO_VERIFY_REQUIRED) return SYM(hello_verify_required);
    if (err == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY) return SYM(peer_close_notify);
    if (err < 0) {
        Throw(exMbedTLSException, (void*) err);
    }
    return MAKEINT(err);
}

// ------------------------------------------------------------------------ //
// * CastAuthMode(RefArg)
// ------------------------------------------------------------------------ //
int
PMbedTLS::CastAuthMode(RefArg inAuthMode)
{
    int authMode = 0;
    if (!IsSymbol(inAuthMode)) {
        ThrowBadTypeWithFrameData(kNSErrNotASymbol, inAuthMode);
    } else if (EQ(inAuthMode, SYM(none))) {
        authMode = MBEDTLS_SSL_VERIFY_NONE;
    } else if (EQ(inAuthMode, SYM(optional))) {
        authMode = MBEDTLS_SSL_VERIFY_OPTIONAL;
    } else if (EQ(inAuthMode, SYM(required))) {
        authMode = MBEDTLS_SSL_VERIFY_REQUIRED;
    } else {
        ThrowBadTypeWithFrameData(kNSErrBadArgs, inAuthMode);
    }
    return authMode;
}

// ------------------------------------------------------------------------ //
// * CastEndpoint(RefArg)
// ------------------------------------------------------------------------ //
int
PMbedTLS::CastEndpoint(RefArg inEndpoint)
{
    int endpoint = 0;
    if (!IsSymbol(inEndpoint)) {
        ThrowBadTypeWithFrameData(kNSErrNotASymbol, inEndpoint);
    }
    if (EQ(inEndpoint, SYM(server))) {
        endpoint = MBEDTLS_SSL_IS_SERVER;
    } else if (EQ(inEndpoint, SYM(client))) {
        endpoint = MBEDTLS_SSL_IS_CLIENT;
    } else {
        ThrowBadTypeWithFrameData(kNSErrBadArgs, inEndpoint);
    }
    return endpoint;
}

// ------------------------------------------------------------------------ //
// * CastTransport(RefArg)
// ------------------------------------------------------------------------ //
int
PMbedTLS::CastTransport(RefArg inTransport)
{
    int transport = 0;
    if (!IsSymbol(inTransport)) {
        ThrowBadTypeWithFrameData(kNSErrNotASymbol, inTransport);
    }
    if (EQ(inTransport, SYM(stream))) {
        transport = MBEDTLS_SSL_TRANSPORT_STREAM;
    } else if (EQ(inTransport, SYM(datagram))) {
        transport = MBEDTLS_SSL_TRANSPORT_DATAGRAM;
    } else {
        ThrowBadTypeWithFrameData(kNSErrBadArgs, inTransport);
    }
    return transport;
}

// ------------------------------------------------------------------------- //
//  * InstallConnectionEnd()
// ------------------------------------------------------------------------- //
Ref
PMbedTLS::InstallConnectionEnd()
{
    TUNameServer NameServer;
    ULong theVersion;
    TClassInfo* theResult = nil;
    NewtonErr theErr = NameServer.Lookup( kInetToolClassInfoNameRegisterFirstString, kInetToolClassInfoNameRegisterSecondString, (ULong*) &theResult, &theVersion );
    if (theErr == kError_Not_Registered) {
        const TClassInfo* inetConnectionEndClassInfo = ClassInfoByName("PConnectionEnd", "PInetToolCE");
        if (inetConnectionEndClassInfo == nil) {
            return NILREF;
        }
        theErr = inetConnectionEndClassInfo->DeRegister();
        if (theErr != noErr) {
            return MAKEINT(theErr);
        }
        theErr = NameServer.RegisterName(kInetToolClassInfoNameRegisterFirstString, kInetToolClassInfoNameRegisterSecondString, (ULong) inetConnectionEndClassInfo, kInetToolClassInfoVersion);
        if (theErr != noErr) {
            return MAKEINT(theErr);
        }
        theErr = PMbedTLSConnectionEnd::ClassInfo()->Register();
        if (theErr != noErr) {
            return MAKEINT(theErr);
        }
        return TRUEREF;
    }
    return NILREF;
}

// ------------------------------------------------------------------------- //
//  * RemoveConnectionEnd()
// ------------------------------------------------------------------------- //
Ref
PMbedTLS::RemoveConnectionEnd()
{
    TUNameServer NameServer;
    ULong theVersion;
    TClassInfo* theResult = nil;
    NewtonErr theErr = NameServer.Lookup( kInetToolClassInfoNameRegisterFirstString, kInetToolClassInfoNameRegisterSecondString, (ULong*) &theResult, &theVersion );
    if (theErr == kError_Not_Registered) {
        return NILREF;
    }
    (void) NameServer.UnRegisterName( kInetToolClassInfoNameRegisterFirstString, kInetToolClassInfoNameRegisterSecondString );
    const TClassInfo* ourOwnClassInfo = ClassInfoByName("PConnectionEnd", "PInetToolCE");
    if (ourOwnClassInfo != PMbedTLSConnectionEnd::ClassInfo()) {
        return MAKEINT(-1);
    }
    theErr = ourOwnClassInfo->DeRegister();
    if (theErr != noErr) {
        return MAKEINT(theErr);
    }
    theErr = theResult->Register();
    if (theErr != noErr) {
        return MAKEINT(theErr);
    }
    return TRUEREF;
}

// ------------------------------------------------------------------------ //
// * HardwarePoll( void*, unsigned char*, size_t, size_t* )
// ------------------------------------------------------------------------ //
// Get some entropy from the NewtonScript entropy collector.
void
PMbedTLS::HardwarePoll( unsigned char *output, size_t len, size_t *olen )
{
    ULong result = 0;

    RefVar context = NSCallGlobalFn(SYM(GetGlobalVar), SYM(mbedTLS:Kallisys));
    RefVar entropyData = NSSend(context, SYM(GetEntropyData), MAKEINT(len));
    if ISNIL(entropyData) {
        *olen = 0;
    } else {
        long numBytes = Length(entropyData);
        numBytes = numBytes > len ? len : numBytes;
        *olen = numBytes;
        WITH_LOCKED_BINARY(entropyData, entropyDataPtr)
            BlockMove(entropyDataPtr, output, numBytes);
        END_WITH_LOCKED_BINARY(entropyData)
    }
}

// ------------------------------------------------------------------------ //
// * NVSeedRead( unsigned char*, size_t )
// ------------------------------------------------------------------------ //
int
PMbedTLS::NVSeedRead( unsigned char *buf, size_t buf_len )
{
    RefVar context = NSCallGlobalFn(SYM(GetGlobalVar), SYM(mbedTLS:Kallisys));
    RefVar seedData = NSSend(context, SYM(NVSeedRead));
    if ISNIL(seedData) {
        return -1;
    }
    long numBytes = Length(seedData);
    numBytes = numBytes > buf_len ? buf_len : numBytes;
    WITH_LOCKED_BINARY(seedData, seedDataPtr)
        BlockMove(seedDataPtr, buf, numBytes);
    END_WITH_LOCKED_BINARY(seedData)

    return numBytes;
}

// ------------------------------------------------------------------------ //
// * NVSeedWrite( unsigned char*, size_t )
// ------------------------------------------------------------------------ //
int
PMbedTLS::NVSeedWrite( unsigned char *buf, size_t buf_len )
{
    RefVar context = NSCallGlobalFn(SYM(GetGlobalVar), SYM(mbedTLS:Kallisys));
    RefVar seedData = AllocateBinary(SYM(binary), buf_len);
    WITH_LOCKED_BINARY(seedData, seedDataPtr)
        BlockMove(buf, seedDataPtr, buf_len);
    END_WITH_LOCKED_BINARY(seedData)
    NSSend(context, SYM(NVSeedWrite), seedData);
    return buf_len;
}

// ------------------------------------------------------------------------ //
// * GetGMTDelta()
// ------------------------------------------------------------------------ //
long
PMbedTLS::GetGMTDelta()
{
    long result;
    RefVar userConfLocation = NSCallGlobalFn(SYM(GetUserConfig), SYM(location));
    if (!ISNIL(userConfLocation)) {
        RefVar gmt = GetFrameSlot(userConfLocation, SYM(gmt));
        if (!ISNIL(gmt)) {
            result = RINT(gmt);
        }
    }
    return result;
}

// ------------------------------------------------------------------------ //
// * GetSerialNumber(unsigned char [8])
// ------------------------------------------------------------------------ //
int
PMbedTLS::GetSerialNumber(unsigned char serialNumber[SERIAL_NUMBER_LEN])
{
    int result;
    newton_try
    {
        RefVar serialNumberBin = NSCall(ROM_GetSerialNumber);
        if (Length(serialNumberBin) == SERIAL_NUMBER_LEN) {
            WITH_LOCKED_BINARY(serialNumberBin, serialNumberBinPtr)
                BlockMove((const unsigned char*) serialNumberBinPtr, serialNumber, SERIAL_NUMBER_LEN);
            END_WITH_LOCKED_BINARY(serialNumberBin)
            result = 0;
        } else {
            result = -1;
        }
    }
    newton_catch (exFrames)
    {
        result = (int) CurrentException()->data;
    }
    newton_catch_all
    {
        result = -1;
    }
    end_try;
    return result;
}

// ========================= //
// Your fault -- core dumped //
// ========================= //
