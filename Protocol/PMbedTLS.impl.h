// =========== Header ===========
// File:                PMbedTLS.impl.h
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

#ifndef __PMBEDTLS__
#define __PMBEDTLS__

#include "PMbedTLSInterface.h"
#include <Newton.h>
#include <NSandDDKIncludes.h>
#include <stdint.h>

// mbedtls
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "mbedtls/x509_crt.h"

#include "NewtonBindings.h"

struct mbedtls_ssl_context;

class TMbedTLSHardwarePollEventHandler;
class TMbedTLSNVSeedReadEventHandler;
class TMbedTLSNVSeedWriteEventHandler;
class TMbedTLSGetGMTDeltaEventHandler;
class TMbedTLSGetSerialNumberEventHandler;
class TMbedTLSListRootCertificatesEventHandler;
class TMbedTLSGetRootCertificateEventHandler;

///
/// mbed TLS protocol.
/// All calls should happen in NewtApp task.
///
PROTOCOL PMbedTLS : public PMbedTLSInterface
    PROTOCOLVERSION(1.0)
{
public:
    PROTOCOL_IMPL_HEADER_MACRO( PMbedTLS );

    ///
    /// Protocol constructor
    ///
    PMbedTLSInterface*      New(void);

    ///
    /// Protocol desctructor.
    ///
    void            Delete (void);

    Ref GetVersion();
    Ref SSLListCiphersuites();

    Ref EntropyNew();

    Ref CtrDrbgNew();
    Ref CtrDrbgSeed(RefArg inCtrDrbgContextBin, RefArg inEntropyContextBin, RefArg inPers);

    Ref PKNew();

    Ref SSLNew();
    Ref SSLCheckPending(RefArg inSSLBin);
    Ref SSLCloseNotify(RefArg inSSLBin);
    Ref SSLGetALPNProtocol(RefArg inSSLBin);
    Ref SSLGetBytesAvail(RefArg inSSLBin);
    Ref SSLGetCiphersuite(RefArg inSSLBin);
    Ref SSLGetMaxFragLen(RefArg inSSLBin);
    Ref SSLGetMaxOutRecordPayload(RefArg inSSLBin);
    Ref SSLGetPeerCert(RefArg inSSLBin);
    Ref SSLGetRecordExpansion(RefArg inSSLBin);
    Ref SSLGetSession(RefArg inSSLBin);
    Ref SSLGetVerifyResult(RefArg inSSLBin);
    Ref SSLGetVersion(RefArg inSSLBin);
    Ref SSLHandshake(RefArg inSSLBin);
    Ref SSLHandshakeStep(RefArg inSSLBin);
    Ref SSLRead(RefArg inSSLBin, RefArg inLen);
    Ref SSLRenegotiate(RefArg inSSLBin);
    Ref SSLSendAlertMessage(RefArg inSSLBin, RefArg inLevel, RefArg inMessage);
    Ref SSLSessionReset(RefArg inSSLBin);
    Ref SSLSetBIO(RefArg inSSLBin, RefArg inSendCb, RefArg inRecvCb);
    Ref SSLSetClientTransportID(RefArg inSSLBin, RefArg inTransportLevelInfo);
    Ref SSLSetDatagramPacking(RefArg inSSLBin, RefArg inAllowPacking);
    Ref SSLSetHostname(RefArg inSSLBin, RefArg inHostname);
    Ref SSLSetHsAuthMode(RefArg inSSLBin, RefArg inAuthmode);
    Ref SSLSetHsCAChain(RefArg inSSLBin, RefArg inX509CrtBin, RefArg inX509CrlBin);
    Ref SSLSetHsOwnCert(RefArg inSSLBin, RefArg inX509CrtBin, RefArg inPKBin);
    Ref SSLSetHsPSK(RefArg inSSLBin, RefArg inPSKBin);
    Ref SSLSetMTU(RefArg inSSLBin, RefArg inMTU);
    Ref SSLSetSession(RefArg inSSLBin, RefArg inSession);
    Ref SSLSetTimerCb(RefArg inSSLBin, RefArg inSetTimerCb, RefArg inGetTimerCb);
    Ref SSLSetup(RefArg inSSLBin, RefArg inSSLConfigBin);
    Ref SSLWrite(RefArg inSSLBin, RefArg inData);

    Ref SSLCacheNew();
    Ref SSLCacheSetMaxEntries(RefArg inSSLCacheBin, RefArg inMaxEntries);
    Ref SSLCacheSetTimeout(RefArg inSSLCacheBin, RefArg inTimeout);

    Ref SSLConfigNew();
    Ref SSLConfALPNProtocol(RefArg inSSLConfigBin, RefArg inProtos);
    Ref SSLConfARC4Support(RefArg inSSLConfigBin, RefArg inARC4);
    Ref SSLConfAuthMode(RefArg inSSLConfigBin, RefArg inAuthMode);
    Ref SSLConfCAChain(RefArg inSSLConfigBin, RefArg inX509CrtBin, RefArg inX509CrlBin);
    Ref SSLConfCbcRecordSplitting(RefArg inSSLConfigBin, RefArg inSplit);
    Ref SSLConfCertProfile(RefArg inSSLConfigBin, RefArg inCrtbin);
    Ref SSLConfCertReqCAList(RefArg inSSLConfigBin, RefArg inCertReqCAList);
    Ref SSLConfCiphersuites(RefArg inSSLConfigBin, RefArg inCiphersuites);
    Ref SSLConfCiphersuitesForVersion(RefArg inSSLConfigBin, RefArg inCiphersuites, RefArg inMajor, RefArg inMinor);
    Ref SSLConfCurves(RefArg inSSLConfigBin, RefArg inCurves);
    Ref SSLConfDbg(RefArg inSSLConfigBin, RefArg inDebugCb);
    Ref SSLConfDHParamBin(RefArg inSSLConfigBin, RefArg inDHMP, RefArg inDHMG);
    Ref SSLConfDHParamCtx(RefArg inSSLConfigBin, RefArg inDHMContextBin);
    Ref SSLConfDHMMinBitlen(RefArg inSSLConfigBin, RefArg inBitlen);
    Ref SSLConfDTLSAntiReplay(RefArg inSSLConfigBin, RefArg inMode);
    Ref SSLConfDTLSBadMACLimit(RefArg inSSLConfigBin, RefArg inLimit);
    Ref SSLConfDTLSCookies(RefArg inSSLConfigBin, RefArg inCookiesWriteCb, RefArg inCookiesCheckCb);
    Ref SSLConfEncryptThenMac(RefArg inSSLConfigBin, RefArg inETM);
    Ref SSLConfEndpoint(RefArg inSSLConfigBin, RefArg inEndpoint);
    Ref SSLConfExportKeysCb(RefArg inSSLConfigBin, RefArg inExportKeysCb);
    Ref SSLConfExtendedMasterSecret(RefArg inSSLConfigBin, RefArg inEMS);
    Ref SSLConfFallback(RefArg inSSLConfigBin, RefArg inFallback);
    Ref SSLConfHandshakeTimeout(RefArg inSSLConfigBin, RefArg inMin, RefArg inMax);
    Ref SSLConfLegacyRenegotiation(RefArg inSSLConfigBin, RefArg inAllowLegacy);
    Ref SSLConfMaxFragLen(RefArg inSSLConfigBin, RefArg inMaxFragLen);
    Ref SSLConfMaxVersion(RefArg inSSLConfigBin, RefArg inMajor, RefArg inMinor);
    Ref SSLConfMinVersion(RefArg inSSLConfigBin, RefArg inMajor, RefArg inMinor);
    Ref SSLConfOwnCert(RefArg inSSLConfigBin, RefArg inCrtBin, RefArg inPkBin);
    Ref SSLConfPSK(RefArg inSSLConfigBin, RefArg inPSK, RefArg inPSKIdentity);
    Ref SSLConfPSKCb(RefArg inSSLConfigBin, RefArg inPSKCb);
    Ref SSLConfRenegotiation(RefArg inSSLConfigBin, RefArg inRenegotiation);
    Ref SSLConfRenegotiationEnforced(RefArg inSSLConfigBin, RefArg inMaxRecords);
    Ref SSLConfRenegotiationPeriod(RefArg inSSLConfigBin, RefArg inPeriod);
    Ref SSLConfRng(RefArg inSSLConfigBin, RefArg inCtrDrbgContextBin);
    Ref SSLConfSessionCache(RefArg inSSLConfigBin, RefArg inCacheBin);
    Ref SSLConfSessionTickets(RefArg inSSLConfigBin, RefArg inUseTickets);
    Ref SSLConfSessionTicketsCb(RefArg inSSLConfigBin, RefArg inTicketWriteCb, RefArg inTicketParseCb);
    Ref SSLConfSigHashes(RefArg inSSLConfigBin, RefArg inSigHashes);
    Ref SSLConfSNI(RefArg inSSLConfigBin, RefArg inSNICb);
    Ref SSLConfTransport(RefArg inSSLConfigBin, RefArg inTransport);
    Ref SSLConfTruncatedHMAC(RefArg inSSLConfigBin, RefArg inTruncate);
    Ref SSLConfVerify(RefArg inSSLConfigBin, RefArg inVerifyCb);
    Ref SSLConfigDefaults(RefArg inSSLConfigBin, RefArg inEndpoint, RefArg inTransport);

    Ref SSLSessionNew();

    Ref X509CrlNew();
    Ref X509CrlParse(RefArg inX509CrlBin, RefArg inCRLBin);

    Ref X509CrtNew();
    Ref X509CrtParse(RefArg inX509CrtBin, RefArg inCertificateBin);
    Ref X509CrtCount(RefArg inX509CrtBin);
    Ref X509CrtGetDER(RefArg inX509CrtBin, RefArg inIndex);
    Ref X509CrtGetVersion(RefArg inX509CrtBin, RefArg inIndex);
    Ref X509CrtGetSerialNumber(RefArg inX509CrtBin, RefArg inIndex);
    Ref X509CrtGetIssuerName(RefArg inX509CrtBin, RefArg inIndex);
    Ref X509CrtGetSubjectName(RefArg inX509CrtBin, RefArg inIndex);
    Ref X509CrtGetIssueDate(RefArg inX509CrtBin, RefArg inIndex);
    Ref X509CrtGetExpirationDate(RefArg inX509CrtBin, RefArg inIndex);
    Ref X509CrtGetSignatureAlgorithm(RefArg inX509CrtBin, RefArg inIndex);

    Ref InstallConnectionEnd();
    Ref RemoveConnectionEnd();

    // Implementations, to be executed in NewtApp task
    static void HardwarePoll(unsigned char *output, size_t len, size_t *olen);
    static int NVSeedRead(unsigned char *buf, size_t buf_len);
    static int NVSeedWrite(unsigned char *buf, size_t buf_len);
    static long GetGMTDelta();
    static int GetSerialNumber(unsigned char buf[SERIAL_NUMBER_LEN]);
    static int GetRootCertificate(const char* name, unsigned char* outBuf, size_t* ioSize);
    static int ListRootCertificates(char* outBuf, size_t* ioSize);

private:
    static void NOPDtor(void*);
    static void EntropyDtor(void*);
    static void CtrDrbgDtor(void*);
    static void PKDtor(void*);
    static void SSLDtor(void*);
    static void SSLCacheDtor(void*);
    static void SSLConfigDtor(void*);
    static void SSLSessionDtor(void*);
    static void X509CrtDtor(void*);
    static void X509CrlDtor(void*);

    Ref DoAllocateCObjectBinary(void* ptr, CObjectBinaryProc dtor);
    static void FreeCObjectBinary(RefArg inCObjectBinary);

    static Ref  CastOrThrowError(int err);
    static int  CastAuthMode(RefArg inAuthMode);
    static int  CastEndpoint(RefArg inEndpoint);
    static int  CastTransport(RefArg inTransport);

    static int  SSLSendCallback(void *ctx, const unsigned char *buf, size_t len);
    static int  SSLRecvCallback(void *ctx, unsigned char *buf, size_t len);
    static void SSLSetTimerCallback(void *ctx, uint32_t int_ms, uint32_t fin_ms);
    static int  SSLGetTimerCallback(void *ctx);
    static int  SSLConfSNICallback(void *ctx, mbedtls_ssl_context *ssl, const unsigned char* hostname, size_t len);
    static int  SSLConfVerifyCallback(void *ctx, mbedtls_x509_crt *crt, int certificate_depth, uint32_t *flags);

    RefStruct*  mPointers;                  // Weak array to keep track of allocated pointers

    TMbedTLSHardwarePollEventHandler* mHardwarePollEventHandler;
    TMbedTLSNVSeedReadEventHandler* mNSSeedReadEventHandler;
    TMbedTLSNVSeedWriteEventHandler* mNSSeedWriteEventHandler;
    TMbedTLSGetGMTDeltaEventHandler* mGetGMTDeltaEventHandler;
    TMbedTLSGetSerialNumberEventHandler* mGetSerialNumberEventHandler;
    TMbedTLSListRootCertificatesEventHandler* mListRootCertificatesEventHandler;
    TMbedTLSGetRootCertificateEventHandler* mGetRootCertificateEventHandler;
};

#endif
        // __PMBEDTLS__

// ============================================= //
// A list is only as strong as its weakest link. //
//                 -- Don Knuth                  //
// ============================================= //
