// =========== Header ===========
// File:                PMbedTLSConnectionEnd.cpp
// Project:             mbed TLS for NewtonOS
// Written by:          Paul Guyot (pguyot@kallisys.net)
//
// Created on:          01/06/2020
// Internal version:    1
//
// Copyright:           © 2020 by Paul Guyot.
//                      All rights reserved worldwide.
// ===========

// =========== Change History ===========
// 01/06/2020   v1  [PG]    Creation of the file
// ===========

#include "PMbedTLSConnectionEnd.impl.h"

// NewtonOS
#include <Newton.h>
#include <CommErrors.h>

// mbedtls
#include "mbedtls/certs.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ssl_cache.h"
#include "mbedtls/version.h"
#include "mbedtls/x509.h"

#include "PMbedTLS.impl.h"
#include "PMbedTLSCECallBack.impl.h"
#include "PMbedTLSCEOptions.h"

// ======================================================================== //
//  Prototypes
// ======================================================================== //

PROTOCOL_IMPL_SOURCE_MACRO(PMbedTLSConnectionEnd)           // Magic stuff, do not touch
PROTOCOL_IMPL_SOURCE_MACRO(PMbedTLSCECallBack)          // Magic stuff, do not touch

#define TRACE_LOG(msg)

extern "C" CBufferList* New__11CBufferListFv(void);
extern "C" CBufferSegment* New__14CBufferSegmentFv(void);

// This may help, but it's not available yet
// mbedtls_x509_crt_parse_der_nocopy
// https://tls.mbed.org/kb/how-to/reduce-mbedtls-memory-and-storage-footprint
// It would require having all certificates in DER format in the package
// and package soup is not an option (binary is copied to heap)
#define ROOT_CERTIFICATES_HEAP_SIZE 512 * 1024

// ------------------------------------------------------------------------- //
//  * Delete()
// ------------------------------------------------------------------------- //
void
PMbedTLSCECallBack::Delete()
{
    TRACE_LOG("PMbedTLSCECallBack::Delete");
}

// ------------------------------------------------------------------------- //
//  * New()
// ------------------------------------------------------------------------- //
PConnectionEnd*
PMbedTLSConnectionEnd::New()
{
    TRACE_LOG("PMbedTLSConnectionEnd::New()");
    TUNameServer NameServer;
    ULong theVersion;
    NewtonErr theErr = NameServer.Lookup( kInetToolClassInfoNameRegisterFirstString, kInetToolClassInfoNameRegisterSecondString, (ULong*) &mInetPConnectionEndClassInfo, &theVersion );
    if (theErr == kError_Not_Registered) {
        return NULL;
    }
    mInetPConnectionEnd = (PConnectionEnd*) mInetPConnectionEndClassInfo->New();
    mThisCECallBack = (PMbedTLSCECallBack*) PMbedTLSCECallBack::ClassInfo()->New();
    mThisCECallBack->Setup(this);
    mSSLConfigured = false;
    mUpgradable = false;
    mInetCECallBack = NULL;
    mPutDataBuffer = NULL;
    mGetDataBuffer = NULL;
    mSendBuffer = NULL;
    mSSLRecvBuffer = NULL;
    mSSLReadBuffer = NULL;
    mAsyncReadMessage = NULL;
    mSSLHostname = NULL;
    mSSLAuthMode = MBEDTLS_SSL_VERIFY_REQUIRED;
    // init consists in filling with zeroes
    mbedtls_entropy_init(&mEntropy);
    mbedtls_ctr_drbg_init(&mCtrDrbg);
    mbedtls_x509_crt_init(&mSSLCAChain);
    mbedtls_ssl_config_init(&mSSLConfig);
    mbedtls_ssl_init(&mSSLContext);
    mSSLLoadRootCertificates = true;
    mRootCertificatesHeap = NULL;
    return this;
}

// ------------------------------------------------------------------------- //
//  * Delete()
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::Delete()
{
    TRACE_LOG("PMbedTLSConnectionEnd::Delete()");
    // This calls FreeInstance, cf PConnectionEndGlue.s
    // Inet PConnection also deletes our callback, so we'll delete passed callback.
    if (mSSLConfigured) {
        SSLUnconfigure();   // Close should have been called.
    }
    mbedtls_ssl_free(&mSSLContext);
    mbedtls_ssl_config_free(&mSSLConfig);
    mbedtls_x509_crt_free(&mSSLCAChain);
    mbedtls_ctr_drbg_free(&mCtrDrbg);
    mbedtls_entropy_free(&mEntropy);
    if (mRootCertificatesHeap) {
        DestroyVMHeap(mRootCertificatesHeap);
        mRootCertificatesHeap = NULL;
    }
    mInetPConnectionEnd->Delete();
    if (mInetCECallBack) {
        mInetCECallBack->Delete();
    }
}

// ------------------------------------------------------------------------- //
//  * Init(PMuxTool*, PCECallBack*, ULong)
// ------------------------------------------------------------------------- //
NewtonErr
PMbedTLSConnectionEnd::Init(PMuxTool* pmuxToolPtr, PCECallBack* ceCallBack, ULong ceNumber)
{
    TRACE_LOG("PMbedTLSConnectionEnd::Init()");
    mInetCECallBack = ceCallBack;
    return mInetPConnectionEnd->Init(pmuxToolPtr, mThisCECallBack, ceNumber);
}

// ------------------------------------------------------------------------- //
//  * HandleInternalRequest(void*)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::HandleInternalRequest(void* msgBuf)
{
    TRACE_LOG("PMbedTLSConnectionEnd::HandleInternalRequest()");
    mInetPConnectionEnd->HandleInternalRequest(msgBuf);
}

// ------------------------------------------------------------------------- //
//  * HandleInternalEvent()
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::HandleInternalEvent()
{
    TRACE_LOG("PMbedTLSConnectionEnd::HandleInternalEvent()");
    mInetPConnectionEnd->HandleInternalEvent();
}

// ------------------------------------------------------------------------- //
//  * OpenStart()
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::OpenStart()
{
    TRACE_LOG("PMbedTLSConnectionEnd::OpenStart()");
    mSSLEnabled = false;
    mSSLConfigured = false;
    mInetPConnectionEnd->OpenStart();
}

// ------------------------------------------------------------------------- //
//  * CloseStart()
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::CloseStart()
{
    TRACE_LOG("PMbedTLSConnectionEnd::CloseStart()");
    mInetPConnectionEnd->CloseStart();
    if (mSSLConfigured) {
        SSLUnconfigure();
    }
}

// ------------------------------------------------------------------------- //
//  * BindStart()
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::BindStart()
{
    TRACE_LOG("PMbedTLSConnectionEnd::BindStart()");
    mInetPConnectionEnd->BindStart();
}

// ------------------------------------------------------------------------- //
//  * UnbindStart()
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::UnbindStart()
{
    TRACE_LOG("PMbedTLSConnectionEnd::UnbindStart()");
    mInetPConnectionEnd->UnbindStart();
}

// ------------------------------------------------------------------------- //
//  * ConnectStart()
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::ConnectStart()
{
    TRACE_LOG("PMbedTLSConnectionEnd::ConnectStart()");
    if (mSSLEnabled) {
        // Configure SSL now.
        int r = SSLConfigure();
        if (r) {
            mInetCECallBack->ConnectComplete(r);
        } else {
            mInetPConnectionEnd->ConnectStart();
        }
    } else {
        mInetPConnectionEnd->ConnectStart();
    }
}

// ------------------------------------------------------------------------- //
//  * ListenStart()
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::ListenStart()
{
    TRACE_LOG("PMbedTLSConnectionEnd::ListenStart()");
    mInetPConnectionEnd->ListenStart();
}

// ------------------------------------------------------------------------- //
//  * AcceptStart()
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::AcceptStart()
{
    TRACE_LOG("PMbedTLSConnectionEnd::AcceptStart()");
    mInetPConnectionEnd->AcceptStart();
}

// ------------------------------------------------------------------------- //
//  * ReleaseStart()
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::ReleaseStart()
{
    TRACE_LOG("PMbedTLSConnectionEnd::ReleaseStart()");
    mInetPConnectionEnd->ReleaseStart();
}

// ------------------------------------------------------------------------- //
//  * TerminateStart()
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::TerminateStart()
{
    TRACE_LOG("PMbedTLSConnectionEnd::TerminateStart()");
    if (mSSLConfigured && mEstablished) {
        (void) mbedtls_ssl_close_notify(&mSSLContext);
        mEstablished = false;
        mClosing = true;
    }
    mInetPConnectionEnd->TerminateStart();
}

// ------------------------------------------------------------------------- //
//  * PutBytesStart(CBufferList*)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::PutBytesStart(CBufferList* clientBuffer)
{
    TRACE_LOG("PMbedTLSConnectionEnd::PutBytesStart()");
    if (mSSLConfigured) {
        if (mClosing) {
            mInetCECallBack->PutComplete(kCommErrConnectionAborted, 0);
        } else {
            // Send to SSL.
            if (mEstablished) {
                int totalSize = clientBuffer->GetSize();
                int pos = clientBuffer->Position();
                int len = totalSize - pos;
                UByte buf[len];
                len = clientBuffer->Getn(buf, len);
                int r = mbedtls_ssl_write(&mSSLContext, buf, len);
                if (r >= 0) {
                    if (r < len) {
                        clientBuffer->Seek(pos + r, -1);
                    }
                    mInetCECallBack->PutComplete(0, r);
                } else {
                    mEstablished = false;
                    mbedtls_ssl_session_reset(&mSSLContext);
                    mInetCECallBack->PutComplete(kCommErrConnectionAborted, 0);
                }
            } else {
                mInetCECallBack->PutComplete(kCommErrNotConnected, 0);
            }
        }
    } else {
        mInetPConnectionEnd->PutBytesStart(clientBuffer);
    }
}

// ------------------------------------------------------------------------- //
//  * PutFramedBytesStart(CBufferList*, Boolean)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::PutFramedBytesStart(CBufferList* clientBuffer, Boolean endOfFrame)
{
    TRACE_LOG("PMbedTLSConnectionEnd::PutFramedBytesStart()");
    if (mSSLConfigured) {
        mInetCECallBack->PutComplete(kCommErrMethodNotImplemented, 0);
    } else {
        mInetPConnectionEnd->PutFramedBytesStart(clientBuffer, endOfFrame);
    }
}

// ------------------------------------------------------------------------- //
//  * KillPutStart()
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::KillPutStart()
{
    TRACE_LOG("PMbedTLSConnectionEnd::KillPutStart()");
    if (mSSLConfigured) {
        mInetCECallBack->KillPutComplete(kCommErrMethodNotImplemented);
    } else {
        mInetPConnectionEnd->KillPutStart();
    }
}

// ------------------------------------------------------------------------- //
//  * GetBytesStart(CBufferList*)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::GetBytesStart(CBufferList* clientBuffer)
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetBytesStart()");
    if (mSSLConfigured) {
        if (mEstablished) {
            if (mSSLReadBuffer != NULL) {
                mInetCECallBack->GetComplete(kCommErrCommandInProgress);
            } else {
                int totalSize = clientBuffer->GetSize();
                int pos = clientBuffer->Position();
                mSSLReadBufferSize = totalSize - pos;
                mSSLReadBuffer = ::NewPtr(mSSLReadBufferSize);
                mSSLReadCount = 0;
                DoSSLRead(clientBuffer, 0);
            }
        } else {
            mInetCECallBack->GetComplete(kCommErrNotConnected);
        }
    } else {
        mInetPConnectionEnd->GetBytesStart(clientBuffer);
    }
}

// ------------------------------------------------------------------------- //
//  * GetFramedBytesStart(CBufferList*)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::GetFramedBytesStart(CBufferList* clientBuffer)
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetFramedBytesStart()");
    if (mSSLConfigured) {
        mInetCECallBack->GetComplete(kCommErrMethodNotImplemented);
    } else {
        mInetPConnectionEnd->GetFramedBytesStart(clientBuffer);
    }
}

// ------------------------------------------------------------------------- //
//  * GetBytesImmediateStart(CBufferList*, Size)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::GetBytesImmediateStart(CBufferList* clientBuffer, Size threshold)
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetBytesImmediateStart()");
    if (mSSLConfigured) {
        if (mEstablished) {
            if (mSSLReadBuffer != NULL) {
                mInetCECallBack->GetComplete(kCommErrCommandInProgress);
            } else {
                int totalSize = clientBuffer->GetSize();
                int pos = clientBuffer->Position();
                mSSLReadBufferSize = totalSize - pos;
                mSSLReadBuffer = ::NewPtr(mSSLReadBufferSize);
                mSSLReadCount = 0;
                DoSSLRead(clientBuffer, threshold);
            }
        } else {
            mInetCECallBack->GetComplete(kCommErrNotConnected);
        }
    } else {
        mInetPConnectionEnd->GetBytesImmediateStart(clientBuffer, threshold);
    }
}

// ------------------------------------------------------------------------- //
//  * KillGetStart()
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::KillGetStart()
{
    TRACE_LOG("PMbedTLSConnectionEnd::KillGetStart()");
    if (mSSLConfigured) {
        mInetCECallBack->KillGetComplete(kCommErrMethodNotImplemented);
    } else {
        mInetPConnectionEnd->KillGetStart();
    }
}

// ------------------------------------------------------------------------- //
//  * HandleTimerTick()
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::HandleTimerTick()
{
    TRACE_LOG("PMbedTLSConnectionEnd::HandleTimerTick()");
    if (mSSLConfigured && !mEstablished && !mClosing && mSSLTimerFin && !mSSLTimerFired) {
        TTime time = GetGlobalTime();
        ULong nowms = time.ConvertTo(kMilliseconds);
        if (nowms >= mSSLTimerFin) {
            DoSSLHandshake();
        }
    }
    mInetPConnectionEnd->HandleTimerTick();
}

// ------------------------------------------------------------------------- //
//  * HandleRequest(UChar*, ULong, TUMsgToken&, ULong)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::HandleRequest(UChar* request, ULong requestSize, TUMsgToken& msgToken, ULong msgType)
{
    TRACE_LOG("PMbedTLSConnectionEnd::HandleRequest()");
    if ((msgType & kMsgType_ReservedMask) == kAsyncReadMessageType) {
        // Based on state, retry handshake or read
        if (!mClosing) {
            if (!mEstablished) {    // retry handshake
                DoSSLHandshake();
            } else if (mSSLReadBuffer != NULL) {    // retry read
                int threshold = 0;
                if (mInetCECallBack->GetCurGetNonBlocking())
                    threshold = mInetCECallBack->GetCurGetThreshold();
                DoSSLRead(mInetCECallBack->GetCurGetData(), threshold);
            }
        }
        mInetCECallBack->HandleRequest(request, requestSize, msgToken, msgType);
    } else {
        mInetPConnectionEnd->HandleRequest(request, requestSize, msgToken, msgType);
    }
}

// ------------------------------------------------------------------------- //
//  * HandleReply(ULong, ULong)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::HandleReply(ULong userRefCon, ULong msgType)
{
    TRACE_LOG("PMbedTLSConnectionEnd::HandleReply()");
    mInetPConnectionEnd->HandleReply(userRefCon, msgType);
}

// ------------------------------------------------------------------------- //
//  * KillControlStart()
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::KillControlStart()
{
    TRACE_LOG("PMbedTLSConnectionEnd::KillControlStart()");
    mInetPConnectionEnd->KillControlStart();
}

// ------------------------------------------------------------------------- //
//  * KillGetCommEventStart()
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::KillGetCommEventStart()
{
    TRACE_LOG("PMbedTLSConnectionEnd::KillGetCommEventStart()");
    mInetPConnectionEnd->KillGetCommEventStart();
}

// ------------------------------------------------------------------------- //
//  * GetCommEvent()
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::GetCommEvent()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetCommEvent()");
    mInetPConnectionEnd->GetCommEvent();
}

// ------------------------------------------------------------------------- //
//  * ProcessOptionStart(TOption*, ULong, ULong)
// ------------------------------------------------------------------------- //
ULong
PMbedTLSConnectionEnd::ProcessOptionStart(TOption* theOption, ULong label, ULong opcode)
{
    TRACE_LOG("PMbedTLSConnectionEnd::ProcessOptionStart()");
    if (label == kSSLEnableLabel) {
        TCMOSSLEnable *o;
        o = (TCMOSSLEnable *) theOption;
        mSSLEnabled = o->enabled;
        if (mUpgradable && mSSLEnabled) {
            int r = SSLConfigure();
            if (r == 0) {
                DoSSLHandshake();
                return opInProgress;
            } else {
                return opFailure;
            }
        }
        return opSuccess;
    }

    if (label == kSSLHostnameLabel) {
        if (mSSLHostname == NULL) {
            TCMOSSLHostname *o;
            o = (TCMOSSLHostname *) theOption;
            int len = strlen((const char*) o->hostname) + 1;
            mSSLHostname = ::NewPtr(len);
            ::BlockMove(o->hostname, mSSLHostname, len);
        }
        return opSuccess;
    }

    if (label == kSSLVerifyModeLabel) {
        TCMOSSLVerifyMode *o;
        o = (TCMOSSLVerifyMode *) theOption;
        mSSLAuthMode = o->verifyMode;
        if (mSSLAuthMode == MBEDTLS_SSL_VERIFY_NONE)
            mSSLLoadRootCertificates = false;   // No need to load root certificates
        return opSuccess;
    }

    if (label == kSSLCertificateAuthorityLabel) {
        int result;
        TCMOSSLCertificate *o;
        o = (TCMOSSLCertificate *) theOption;
        mSSLLoadRootCertificates = false;
        // Root certificates can be large.
        // Allocate them in a separate heap.
        if (mRootCertificatesHeap == NULL) {
            NewtonErr theErr = NewVMHeap(0, ROOT_CERTIFICATES_HEAP_SIZE, &mRootCertificatesHeap, 0);
            if (theErr != noErr) return theErr;
        }
        Heap callerHeap = GetHeap();
        SetHeap(mRootCertificatesHeap);
        unwind_protect {
            result = mbedtls_x509_crt_parse(&mSSLCAChain, o->certificate, o->length);
        } on_unwind {
            SetHeap(callerHeap);
        } end_unwind;
        if (result < 0) {
            return opFailure;
        }
        return opSuccess;
    }

    if (label == kSSLPeerCertificateLabel) {
        if (mSSLConfigured && mEstablished) {
            TCMOSSLCertificate * peerCertificateOption = (TCMOSSLCertificate *) theOption;
            const mbedtls_x509_crt* peer_cert = mbedtls_ssl_get_peer_cert(&mSSLContext);
            ULong ix = peerCertificateOption->indexInChain;
            while (ix > 0) {
                peer_cert = peer_cert->next;
                if (peer_cert == NULL) return opFailure;
                ix--;
            }
            int certSize = peer_cert->raw.len;
            if (certSize <= peerCertificateOption->length) {
                ::BlockMove(peer_cert->raw.p, peerCertificateOption->certificate, certSize);
            }
            peerCertificateOption->length = certSize;
            return opSuccess;
        }
        return opNotSupported;
    }
    ULong r = mInetPConnectionEnd->ProcessOptionStart(theOption, label, opcode);
    return r;
}

// ------------------------------------------------------------------------- //
//  * ProcessPutBytesOptionStart(TOption*, ULong, ULong)
// ------------------------------------------------------------------------- //
ULong
PMbedTLSConnectionEnd::ProcessPutBytesOptionStart(TOption* theOption, ULong label, ULong opcode)
{
    TRACE_LOG("PMbedTLSConnectionEnd::ProcessPutBytesOptionStart()");
    return mInetPConnectionEnd->ProcessPutBytesOptionStart(theOption, label, opcode);
}

// ------------------------------------------------------------------------- //
//  * ProcessGetBytesOptionStart(TOption*, ULong, ULong)
// ------------------------------------------------------------------------- //
ULong
PMbedTLSConnectionEnd::ProcessGetBytesOptionStart(TOption* theOption, ULong label, ULong opcode)
{
    TRACE_LOG("PMbedTLSConnectionEnd::ProcessGetBytesOptionStart()");
    return mInetPConnectionEnd->ProcessGetBytesOptionStart(theOption, label, opcode);
}

// ------------------------------------------------------------------------- //
//  * ResArbReleaseStart(UChar*, UChar*)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::ResArbReleaseStart(UChar* resName, UChar* resType)
{
    TRACE_LOG("PMbedTLSConnectionEnd::ResArbReleaseStart()");
    mInetPConnectionEnd->ResArbReleaseStart(resName, resType);
}

// ------------------------------------------------------------------------- //
//  * ResArbClaimNotification(UChar*, UChar*)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::ResArbClaimNotification(UChar* resName, UChar* resType)
{
    TRACE_LOG("PMbedTLSConnectionEnd::ResArbClaimNotification()");
    mInetPConnectionEnd->ResArbClaimNotification(resName, resType);
}

// ------------------------------------------------------------------------- //
//  * ForwardOptions()
// ------------------------------------------------------------------------- //
TUPort*
PMbedTLSConnectionEnd::ForwardOptions()
{
    TRACE_LOG("PMbedTLSConnectionEnd::ForwardOptions()");
    return mInetPConnectionEnd->ForwardOptions();
}

// ------------------------------------------------------------------------- //
//  * InitCB(TConnectionEnd* cePtr)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::InitCB(TConnectionEnd* cePtr)
{
    TRACE_LOG("PMbedTLSConnectionEnd::InitCB()");
    mInetCECallBack->Init(cePtr);
}

// ------------------------------------------------------------------------- //
//  * OpenCompleteCB(NewtonErr)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::OpenCompleteCB(NewtonErr result)
{
    TRACE_LOG("PMbedTLSConnectionEnd::OpenCompleteCB()");
    mInetCECallBack->OpenComplete(result);
}

// ------------------------------------------------------------------------- //
//  * CloseCompleteCB(NewtonErr)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::CloseCompleteCB(NewtonErr result)
{
    TRACE_LOG("PMbedTLSConnectionEnd::CloseCompleteCB()");
    mInetCECallBack->CloseComplete(result);
}

// ------------------------------------------------------------------------- //
//  * BindCompleteCB(NewtonErr result)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::BindCompleteCB(NewtonErr result)
{
    TRACE_LOG("PMbedTLSConnectionEnd::BindCompleteCB()");
    mInetCECallBack->BindComplete(result);
}

// ------------------------------------------------------------------------- //
//  * UnbindCompleteCB(NewtonErr result)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::UnbindCompleteCB(NewtonErr result)
{
    TRACE_LOG("PMbedTLSConnectionEnd::UnbindCompleteCB()");
    mInetCECallBack->UnbindComplete(result);
}


// ------------------------------------------------------------------------- //
//  * ConnectCompleteCB(NewtonErr result)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::ConnectCompleteCB(NewtonErr result)
{
    TRACE_LOG("PMbedTLSConnectionEnd::ConnectCompleteCB()");
    if (mSSLEnabled && result == 0) {
        DoSSLHandshake();
    } else {
        if (result == 0) mUpgradable = true;
        mInetCECallBack->ConnectComplete(result);
    }
}

// ------------------------------------------------------------------------- //
//  * ListenCompleteCB(NewtonErr result)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::ListenCompleteCB(NewtonErr result)
{
    TRACE_LOG("PMbedTLSConnectionEnd::ListenCompleteCB()");
    mInetCECallBack->ListenComplete(result);
}

// ------------------------------------------------------------------------- //
//  * AcceptCompleteCB(NewtonErr result)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::AcceptCompleteCB(NewtonErr result)
{
    TRACE_LOG("PMbedTLSConnectionEnd::AcceptCompleteCB()");
    mInetCECallBack->AcceptComplete(result);
}


// ------------------------------------------------------------------------- //
//  * TerminateCompleteCB()
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::TerminateCompleteCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::TerminateCompleteCB()");
    mInetCECallBack->TerminateComplete();
}


// ------------------------------------------------------------------------- //
//  * PutCompleteCB(NewtonErr result, ULong putBytesCount)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::PutCompleteCB(NewtonErr result, ULong putBytesCount)
{
    TRACE_LOG("PMbedTLSConnectionEnd::PutCompleteCB()");
    if (mSSLConfigured) {
        // We are no longer waiting for a receive
        mSendPending = false;
        if (result != noErr) {
            mSendError = result;
        } else {
            if (mPutDataBuffer->AtEOF()) {
                mPutDataBuffer->DeleteAll();
                bool moreSegments = false;
                do {
                    CBufferSegment* segment = (CBufferSegment*) mSendBuffer->First();
                    if (segment == NULL) break;
                    mSendBuffer->Remove((CBuffer*) segment);
                    segment->Reset();
                    mPutDataBuffer->InsertLast((CBuffer*) segment);
                    moreSegments = true;
                } while (1);
                if (moreSegments) {
                    mSendPending = true;
                    mInetPConnectionEnd->PutBytesStart(mPutDataBuffer);
                }
            } else {
                mSendPending = true;
                mInetPConnectionEnd->PutBytesStart(mPutDataBuffer);
            }
        }
    } else {
        mInetCECallBack->PutComplete(result, putBytesCount);
    }
}

// ------------------------------------------------------------------------- //
//  * KillPutCompleteCB(NewtonErr result)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::KillPutCompleteCB(NewtonErr result)
{
    TRACE_LOG("PMbedTLSConnectionEnd::KillPutCompleteCB()");
    mInetCECallBack->KillPutComplete(result);
}


// ------------------------------------------------------------------------- //
//  * GetCompleteCB(NewtonErr, Boolean, ULong)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::GetCompleteCB(NewtonErr result, Boolean endOfFrame, ULong getBytesCount)
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetCompleteCB()");
    if (mSSLConfigured) {
        // We are no longer waiting for a receive
        mRecvPending = false;
        if (result == 0) {
            if (getBytesCount == 0) {
                // EOF.
                mPeerClosed = true;
            } else {
                // Copy data from GetDataBuffer to SendBuffer.
                if (mSSLRecvBuffer) {
                    mSSLRecvBuffer = ::ReallocPtr(mSSLRecvBuffer, mSSLRecvBufferSize + getBytesCount);
                } else {
                    mSSLRecvBuffer = ::NewPtr(getBytesCount);
                    mSSLRecvBufferSize = 0;
                }
                mGetDataBuffer->Reset();
                mGetDataBuffer->Getn((UByte*) mSSLRecvBuffer + mSSLRecvBufferSize, getBytesCount);
                mGetDataBuffer->DeleteAll();
                mSSLRecvBufferSize += getBytesCount;
            }
        } else {
            mRecvError = result;
        }
        if (!mSSLReceiveInProgress) {
            // Send an event to retry ssl handshake or ssl read
            // Send may fail if event is already there (which is ok)
            (void) mInetCEPort->Send(mAsyncReadMessage, mAsyncReadMessage, sizeof(*mAsyncReadMessage), kNoTimeout, nil, kAsyncReadMessageType, false);
        }
    } else {
        mInetCECallBack->GetComplete(result, endOfFrame, getBytesCount);
    }
}

// ------------------------------------------------------------------------- //
//  * KillGetCompleteCB(NewtonErr result)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::KillGetCompleteCB(NewtonErr result)
{
    TRACE_LOG("PMbedTLSConnectionEnd::KillGetCompleteCB()");
    mInetCECallBack->KillGetComplete(result);
}


// ------------------------------------------------------------------------- //
//  * HandleRequestCB(UChar*, ULong, TUMsgToken&, ULong)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::HandleRequestCB(UChar* request, ULong requestSize, TUMsgToken& msgToken, ULong msgType)
{
    TRACE_LOG("PMbedTLSConnectionEnd::HandleRequestCB()");
    mInetCECallBack->HandleRequest(request, requestSize, msgToken, msgType);
}

// ------------------------------------------------------------------------- //
//  * HandleReplyCB(ULong userRefCon, ULong msgType)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::HandleReplyCB(ULong userRefCon, ULong msgType)
{
    TRACE_LOG("PMbedTLSConnectionEnd::HandleReplyCB()");
    mInetCECallBack->HandleReply(userRefCon, msgType);
}


// ------------------------------------------------------------------------- //
//  * KillControlCompleteCB(NewtonErr result)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::KillControlCompleteCB(NewtonErr result)
{
    TRACE_LOG("PMbedTLSConnectionEnd::KillControlCompleteCB()");
    mInetCECallBack->KillControlComplete(result);
}

// ------------------------------------------------------------------------- //
//  * KillGetCommEventCompleteCB(NewtonErr result)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::KillGetCommEventCompleteCB(NewtonErr result)
{
    TRACE_LOG("PMbedTLSConnectionEnd::KillGetCommEventCompleteCB()");
    mInetCECallBack->KillGetCommEventComplete(result);
}


// ------------------------------------------------------------------------- //
//  * GetCommEventCB()
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::GetCommEventCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetCommEventCB()");
    mInetCECallBack->GetCommEvent();
}

// ------------------------------------------------------------------------- //
//  * PostCommEventCB(TCommToolGetEventReply& theEvent, NewtonErr result)
// ------------------------------------------------------------------------- //
NewtonErr
PMbedTLSConnectionEnd::PostCommEventCB(TCommToolGetEventReply& theEvent, NewtonErr result)
{
    TRACE_LOG("PMbedTLSConnectionEnd::PostCommEventCB()");
    return mInetCECallBack->PostCommEvent(theEvent, result);
}


// ------------------------------------------------------------------------- //
//  * ProcessOptionCompleteCB(ULong optResult)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::ProcessOptionCompleteCB(ULong optResult)
{
    TRACE_LOG("PMbedTLSConnectionEnd::ProcessOptionCompleteCB()");
    mInetCECallBack->ProcessOptionComplete(optResult);
}

// ------------------------------------------------------------------------- //
//  * ProcessPutBytesOptionCompleteCB(ULong optResult)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::ProcessPutBytesOptionCompleteCB(ULong optResult)
{
    TRACE_LOG("PMbedTLSConnectionEnd::ProcessPutBytesOptionCompleteCB()");
    mInetCECallBack->ProcessPutBytesOptionComplete(optResult);
}

// ------------------------------------------------------------------------- //
//  * ProcessGetBytesOptionCompleteCB(ULong optResult)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::ProcessGetBytesOptionCompleteCB(ULong optResult)
{
    TRACE_LOG("PMbedTLSConnectionEnd::ProcessGetBytesOptionCompleteCB()");
    mInetCECallBack->ProcessGetBytesOptionComplete(optResult);
}


// ------------------------------------------------------------------------- //
//  * ResArbReleaseCompleteCB(NewtonErr result)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::ResArbReleaseCompleteCB(NewtonErr result)
{
    TRACE_LOG("PMbedTLSConnectionEnd::ResArbReleaseCompleteCB()");
    mInetCECallBack->ResArbReleaseComplete(result);
}


// ------------------------------------------------------------------------- //
//  * FlushChannelCB(ULong filter, NewtonErr flushResult)
// ------------------------------------------------------------------------- //
NewtonErr
PMbedTLSConnectionEnd::FlushChannelCB(ULong filter, NewtonErr flushResult)
{
    TRACE_LOG("PMbedTLSConnectionEnd::FlushChannelCB()");
    return mInetCECallBack->FlushChannel(filter, flushResult);
}

// ------------------------------------------------------------------------- //
//  * StartAbortCB(NewtonErr abortError)
// ------------------------------------------------------------------------- //
NewtonErr
PMbedTLSConnectionEnd::StartAbortCB(NewtonErr abortError)
{
    TRACE_LOG("PMbedTLSConnectionEnd::StartAbortCB()");
    return mInetCECallBack->StartAbort(abortError);
}

// ------------------------------------------------------------------------- //
//  * GetAbortErrCB()
// ------------------------------------------------------------------------- //
NewtonErr
PMbedTLSConnectionEnd::GetAbortErrCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetAbortErrCB()");
    return mInetCECallBack->GetAbortErr();
}

// ------------------------------------------------------------------------- //
//  * GetTerminationEventCB()
// ------------------------------------------------------------------------- //
ULong
PMbedTLSConnectionEnd::GetTerminationEventCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetTerminationEventCB()");
    return mInetCECallBack->GetTerminationEvent();
}

// ------------------------------------------------------------------------- //
//  * SetTerminationEventCB(ULong event)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::SetTerminationEventCB(ULong event)
{
    TRACE_LOG("PMbedTLSConnectionEnd::SetTerminationEventCB()");
    mInetCECallBack->SetTerminationEvent(event);
}


// ------------------------------------------------------------------------- //
//  * InitAsyncRPCMsgCB(TUAsyncMessage& asyncMsg, ULong refCon)
// ------------------------------------------------------------------------- //
NewtonErr
PMbedTLSConnectionEnd::InitAsyncRPCMsgCB(TUAsyncMessage& asyncMsg, ULong refCon)
{
    TRACE_LOG("PMbedTLSConnectionEnd::InitAsyncRPCMsgCB()");
    return mInetCECallBack->InitAsyncRPCMsg(asyncMsg, refCon);
}

// ------------------------------------------------------------------------- //
//  * GetToolConnectStateCB()
// ------------------------------------------------------------------------- //
ULong
PMbedTLSConnectionEnd::GetToolConnectStateCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetToolConnectStateCB()");
    return mInetCECallBack->GetToolConnectState();
}

// ------------------------------------------------------------------------- //
//  * SetToolConnectStateCB(ULong state)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::SetToolConnectStateCB(ULong state)
{
    TRACE_LOG("PMbedTLSConnectionEnd::SetToolConnectStateCB()");
    mInetCECallBack->SetToolConnectState(state);
}


// ------------------------------------------------------------------------- //
//  * GetConnectInfoCB()
// ------------------------------------------------------------------------- //
TCMOCTConnectInfo&
PMbedTLSConnectionEnd::GetConnectInfoCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetConnectInfoCB()");
    return mInetCECallBack->GetConnectInfo();
}

// ------------------------------------------------------------------------- //
//  * SetConnectInfoCB(TCMOCTConnectInfo& info)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::SetConnectInfoCB(TCMOCTConnectInfo& info)
{
    TRACE_LOG("PMbedTLSConnectionEnd::SetConnectInfoCB()");
    mInetCECallBack->SetConnectInfo(info);
}


// ------------------------------------------------------------------------- //
//  * GetCMOTransportInfoCB()
// ------------------------------------------------------------------------- //
TCMOTransportInfo&
PMbedTLSConnectionEnd::GetCMOTransportInfoCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetCMOTransportInfoCB()");
    return mInetCECallBack->GetCMOTransportInfo();
}

// ------------------------------------------------------------------------- //
//  * SetCMOTransportInfoCB(TCMOTransportInfo& info)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::SetCMOTransportInfoCB(TCMOTransportInfo& info)
{
    TRACE_LOG("PMbedTLSConnectionEnd::SetCMOTransportInfoCB()");
    mInetCECallBack->SetCMOTransportInfo(info);
}


// ------------------------------------------------------------------------- //
//  * GetReceiveMessageBufSizeCB()
// ------------------------------------------------------------------------- //
// returns the size of the buffer used by the commtool to receive rpc messages
// Any message sent to the tool which exceeds this size will be truncated.
ULong
PMbedTLSConnectionEnd::GetReceiveMessageBufSizeCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetReceiveMessageBufSizeCB()");
    return mInetCECallBack->GetReceiveMessageBufSize();
}

// ------------------------------------------------------------------------- //
//  * GetConnectionEndPortCB()
// ------------------------------------------------------------------------- //
// returns a pointer to the tool's port object
TUPort*
PMbedTLSConnectionEnd::GetConnectionEndPortCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetConnectionEndPortCB()");
    return mInetCECallBack->GetConnectionEndPort();
}


// ------------------------------------------------------------------------- //
//  * GetControlOptionsInfoCB()
// ------------------------------------------------------------------------- //
// returns the option info for the current control request
TCommToolOptionInfo&
PMbedTLSConnectionEnd::GetControlOptionsInfoCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetControlOptionsInfoCB()");
    return mInetCECallBack->GetControlOptionsInfo();
}

// ------------------------------------------------------------------------- //
//  * GetGetBytesOptionsInfoCB()
// ------------------------------------------------------------------------- //
// returns the option info for the current get bytes request
TCommToolOptionInfo&
PMbedTLSConnectionEnd::GetGetBytesOptionsInfoCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetGetBytesOptionsInfoCB()");
    return mInetCECallBack->GetGetBytesOptionsInfo();
}

// ------------------------------------------------------------------------- //
//  * GetPutBytesOptionsInfoCB()
// ------------------------------------------------------------------------- //
// returns the option info for the current put bytes request
TCommToolOptionInfo&
PMbedTLSConnectionEnd::GetPutBytesOptionsInfoCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetPutBytesOptionsInfoCB()");
    return mInetCECallBack->GetPutBytesOptionsInfo();
}


// ------------------------------------------------------------------------- //
//  * GetCurPutDataCB()
// ------------------------------------------------------------------------- //
// returns a pointer to the current put request buffer list
CBufferList*
PMbedTLSConnectionEnd::GetCurPutDataCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetCurPutDataCB()");
    if (mSSLConfigured) {
        return mPutDataBuffer;
    } else {
        return mInetCECallBack->GetCurPutData();
    }
}

// ------------------------------------------------------------------------- //
//  * GetCurPutFrameDataCB()
// ------------------------------------------------------------------------- //
// returns true if current put request is framed
Boolean
PMbedTLSConnectionEnd::GetCurPutFrameDataCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetCurPutFrameDataCB()");
    if (mSSLConfigured) {
        return false;
    } else {
        return mInetCECallBack->GetCurPutFrameData();
    }
}

// ------------------------------------------------------------------------- //
//  * GetCurPutEndOfFrameCB()
// ------------------------------------------------------------------------- //
// returns true if current framed put request is end of frame
Boolean
PMbedTLSConnectionEnd::GetCurPutEndOfFrameCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetCurPutEndOfFrameCB()");
    if (mSSLConfigured) {
        return false;
    } else {
        return mInetCECallBack->GetCurPutEndOfFrame();
    }
}


// ------------------------------------------------------------------------- //
//  * GetCurGetDataCB()
// ------------------------------------------------------------------------- //
// returns a pointer to the current get request buffer list
CBufferList*
PMbedTLSConnectionEnd::GetCurGetDataCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetCurGetDataCB()");
    if (mSSLConfigured) {
        return mGetDataBuffer;
    } else {
        return mInetCECallBack->GetCurGetData();
    }
}

// ------------------------------------------------------------------------- //
//  * GetCurGetFrameDataCB()
// ------------------------------------------------------------------------- //
// returns true if current get request is framed
Boolean
PMbedTLSConnectionEnd::GetCurGetFrameDataCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetCurGetFrameDataCB()");
    if (mSSLConfigured) {
        return false;
    } else {
        return mInetCECallBack->GetCurGetFrameData();
    }
}

// ------------------------------------------------------------------------- //
//  * GetCurGetNonBlockingCB()
// ------------------------------------------------------------------------- //
// returns true if current get request is non blocking (ie has a threshold)
Boolean
PMbedTLSConnectionEnd::GetCurGetNonBlockingCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetCurGetNonBlockingCB()");
    if (mSSLConfigured) {
        return false;
    } else {
        return mInetCECallBack->GetCurGetNonBlocking();
    }
}

// ------------------------------------------------------------------------- //
//  * GetCurGetThresholdCB()
// ------------------------------------------------------------------------- //
// returns the value of threshold for the current nonblocking get request
Size
PMbedTLSConnectionEnd::GetCurGetThresholdCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetCurGetThresholdCB()");
    if (mSSLConfigured) {
        return 0;
    } else {
        return mInetCECallBack->GetCurGetThreshold();
    }
}


// ------------------------------------------------------------------------- //
//  * GetPassiveClaimCB()
// ------------------------------------------------------------------------- //
// returns true if client asked for passive claim of tool resources
Boolean
PMbedTLSConnectionEnd::GetPassiveClaimCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetPassiveClaimCB()");
    return mInetCECallBack->GetPassiveClaim();
}

// ------------------------------------------------------------------------- //
//  * SetPassiveClaimCB(Boolean passiveClaim)
// ------------------------------------------------------------------------- //
// Set the value of passive claim
void
PMbedTLSConnectionEnd::SetPassiveClaimCB(Boolean passiveClaim)
{
    TRACE_LOG("PMbedTLSConnectionEnd::SetPassiveClaimCB()");
    return mInetCECallBack->SetPassiveClaim(passiveClaim);
}

// ------------------------------------------------------------------------- //
//  * GetPassiveStateCB()
// ------------------------------------------------------------------------- //
// returns true if in passive state (resources passively claimed, and willing to give up resources)
Boolean
PMbedTLSConnectionEnd::GetPassiveStateCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetPassiveStateCB()");
    return mInetCECallBack->GetPassiveState();
}

// ------------------------------------------------------------------------- //
//  * SetPassiveStateCB(Boolean passiveState)
// ------------------------------------------------------------------------- //
// Set the value of passive claim
void
PMbedTLSConnectionEnd::SetPassiveStateCB(Boolean passiveState)
{
    TRACE_LOG("PMbedTLSConnectionEnd::SetPassiveStateCB()");
    return mInetCECallBack->SetPassiveState(passiveState);
}


// ------------------------------------------------------------------------- //
//  * GetWaitingForResNotifyCB()
// ------------------------------------------------------------------------- //
// returns true if tool has passively claimed resources, and is waiting for notification of resource ownership
Boolean
PMbedTLSConnectionEnd::GetWaitingForResNotifyCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetWaitingForResNotifyCB()");
    return mInetCECallBack->GetWaitingForResNotify();
}

// ------------------------------------------------------------------------- //
//  * SetWaitingForResNotifyCB(Boolean waitingForResNotify)
// ------------------------------------------------------------------------- //
// Set the value of passive claim
void
PMbedTLSConnectionEnd::SetWaitingForResNotifyCB(Boolean waitingForResNotify)
{
    TRACE_LOG("PMbedTLSConnectionEnd::SetWaitingForResNotifyCB()");
    return mInetCECallBack->SetWaitingForResNotify(waitingForResNotify);
}


// ------------------------------------------------------------------------- //
//  * GetCurRequestOpCodeCB()
// ------------------------------------------------------------------------- //
// returns the opcode of the current control request
ULong
PMbedTLSConnectionEnd::GetCurRequestOpCodeCB()
{
    TRACE_LOG("PMbedTLSConnectionEnd::GetCurRequestOpCodeCB()");
    return mInetCECallBack->GetCurRequestOpCode();
}

// ------------------------------------------------------------------------- //
//  * DoSSLRead(CBufferList*, int)
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::DoSSLRead(CBufferList* clientBuffer, int threshold) {
    TRACE_LOG("PMbedTLSConnectionEnd::DoSSLRead()");
    while (true) {
        int r = mbedtls_ssl_read(&mSSLContext, (unsigned char*) mSSLReadBuffer + mSSLReadCount, mSSLReadBufferSize - mSSLReadCount);
        if (r >= 0) {
            mSSLReadCount += r;
            if (mSSLReadCount >= threshold) {
                Size copied = clientBuffer->Putn((const UByte*) mSSLReadBuffer, mSSLReadCount);
                ::DisposPtr(mSSLReadBuffer);
                mSSLReadBuffer = NULL;
                mSSLReadBufferSize = 0;
                mInetCECallBack->GetComplete(0, true, mSSLReadCount);
                mSSLReadCount = 0;
                break;
            }
        } else if (r == MBEDTLS_ERR_SSL_WANT_READ || r == MBEDTLS_ERR_SSL_WANT_WRITE) {
            break;
        } else {
            NewtonErr err = r;
            if (r == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY) {
                int r = mbedtls_ssl_close_notify(&mSSLContext);
                err = 0;    // Signal EOF
            } else if (r == MBEDTLS_ERR_SSL_CONN_EOF) {
                err = 0;
            }
            mEstablished = false;
            mClosing = true;
            if (mSSLReadCount > 0) {
                (void) clientBuffer->Putn((const UByte*) mSSLReadBuffer, mSSLReadCount);
                ::DisposPtr(mSSLReadBuffer);
                mSSLReadBuffer = NULL;
                mSSLReadBufferSize = 0;
            }
            mbedtls_ssl_session_reset(&mSSLContext);
            mInetCECallBack->GetComplete(err, true, mSSLReadCount);
            break;
        }
    }
}

// ------------------------------------------------------------------------- //
//  * DoSSLHandshake()
// ------------------------------------------------------------------------- //
void
PMbedTLSConnectionEnd::DoSSLHandshake() {
    TRACE_LOG("PMbedTLSConnectionEnd::DoSSLHandshake()");
    int r = mbedtls_ssl_handshake(&mSSLContext);
    if (r == 0) {
        mEstablished = true;
        if (mUpgradable) {
            mInetCECallBack->ProcessOptionComplete(opSuccess);
        } else {
            mInetCECallBack->ConnectComplete(0);
        }
    } else if (r != MBEDTLS_ERR_SSL_WANT_READ && r != MBEDTLS_ERR_SSL_WANT_WRITE) {
        mEstablished = false;
        mClosing = true;
        mbedtls_ssl_session_reset(&mSSLContext);
        mInetCECallBack->ConnectComplete(r);
    }
}

// ------------------------------------------------------------------------ //
// * SSLSendCallback(void *, const unsigned char *, size_t)
// ------------------------------------------------------------------------ //
int
PMbedTLSConnectionEnd::SSLSendCallback(void *ctx, const unsigned char *buf, size_t len)
{
    TRACE_LOG("SSLSendCallback()");
    PMbedTLSConnectionEnd* self = (PMbedTLSConnectionEnd*) ctx;
    if (self->mSendError != noErr) {
        return self->mSendError;
    }
    CBufferSegment* segment = New__14CBufferSegmentFv();
    char* ptr = ::NewPtr(len);
    ::BlockMove(buf, ptr, len);
    segment->Init((void*) ptr, len, true);
    self->mSendBuffer->InsertLast((CBuffer*) segment);
    if (!self->mSendPending) {
        do {
            CBufferSegment* segment = (CBufferSegment*) self->mSendBuffer->First();
            if (segment == NULL) break;
            self->mSendBuffer->Remove((CBuffer*) segment);
            segment->Reset();
            self->mPutDataBuffer->InsertLast((CBuffer*) segment);
        } while (true);
        self->mSendPending = true;
        self->mInetPConnectionEnd->PutBytesStart(self->mPutDataBuffer);
    }
    return len;
}

// ------------------------------------------------------------------------ //
// * SSLRecvCallback(void *, unsigned char *, size_t)
// ------------------------------------------------------------------------ //
int
PMbedTLSConnectionEnd::SSLRecvCallback(void *ctx, unsigned char *buf, size_t len)
{
    TRACE_LOG("SSLRecvCallback()");
    int result;
    PMbedTLSConnectionEnd* self = (PMbedTLSConnectionEnd*) ctx;
    if (self->mRecvError != noErr) {
        result = self->mRecvError;
    } else if (self->mPeerClosed) {
        result = 0;
    } else {
        if (self->mSSLRecvBufferSize < len && !self->mRecvPending) {
            self->mRecvPending = true;
            CBufferSegment* segment = New__14CBufferSegmentFv();
            segment->Init(len - self->mSSLRecvBufferSize);
            self->mGetDataBuffer->InsertLast((CBuffer*) segment);
            // mbedtls knows exactly how many bytes we should expect, use this as a threshold
            self->mSSLReceiveInProgress = true;
            self->mInetPConnectionEnd->GetBytesImmediateStart(self->mGetDataBuffer, 1);
            self->mSSLReceiveInProgress = false;
        }
        if (self->mRecvError != noErr) {
            result = self->mRecvError;
        } else if (self->mSSLRecvBufferSize == 0) {
            result = MBEDTLS_ERR_SSL_WANT_READ;
        } else {
            Size copied = self->mSSLRecvBufferSize > len ? len : self->mSSLRecvBufferSize;
            ::BlockMove((const char*) self->mSSLRecvBuffer, buf, copied);
            if (copied == self->mSSLRecvBufferSize) {
                ::DisposPtr((Ptr) self->mSSLRecvBuffer);
                self->mSSLRecvBuffer = NULL;
                self->mSSLRecvBufferSize = 0;
            } else {
                ::BlockMove((const char*) self->mSSLRecvBuffer + self->mSSLRecvBufferSize, self->mSSLRecvBuffer, self->mSSLRecvBufferSize - copied);
                self->mSSLRecvBufferSize -= copied;
                self->mSSLRecvBuffer = ::ReallocPtr(self->mSSLRecvBuffer, self->mSSLRecvBufferSize);
            }
            result = copied;
        }
    }
    return result;
}

// ------------------------------------------------------------------------ //
// * SSLSetTimerCallback(void *)
// ------------------------------------------------------------------------ //
void
PMbedTLSConnectionEnd::SSLSetTimerCallback(void *ctx, uint32_t int_ms, uint32_t fin_ms)
{
    TRACE_LOG("PMbedTLSConnectionEnd::SSLSetTimerCallback()");
    PMbedTLSConnectionEnd* self = (PMbedTLSConnectionEnd*) ctx;
    if (fin_ms == 0) {
        self->mSSLTimerInt = 0;
        self->mSSLTimerFin = 0;
        self->mSSLTimerFired = false;
    } else {
        TTime time = GetGlobalTime();
        ULong nowms = time.ConvertTo(kMilliseconds);
        self->mSSLTimerInt = int_ms + nowms;
        self->mSSLTimerFin = fin_ms + nowms;
        self->mSSLTimerFired = false;
    }
}

// ------------------------------------------------------------------------ //
// * SSLSetTimerCallback(void *)
// ------------------------------------------------------------------------ //
int
PMbedTLSConnectionEnd::SSLGetTimerCallback(void *ctx)
{
    int ret;
    PMbedTLSConnectionEnd* self = (PMbedTLSConnectionEnd*) ctx;
    if (self->mSSLTimerFin == 0) {
        ret = -1;
    } else {
        TTime time = GetGlobalTime();
        ULong nowms = time.ConvertTo(kMilliseconds);
        if (nowms >= self->mSSLTimerFin) {
            ret = 2;
        } else if (nowms >= self->mSSLTimerInt) {
            ret = 1;
        } else {
            ret = 0;
        }
    }
    return ret;
}

// ------------------------------------------------------------------------ //
// * SSLConfigure()
// ------------------------------------------------------------------------ //
int
PMbedTLSConnectionEnd::SSLConfigure()
{
    TRACE_LOG("PMbedTLSConnectionEnd::SSLConfigure");
    mInetCEPort = mInetCECallBack->GetConnectionEndPort();
    mbedtls_entropy_add_source(&mEntropy, mbedtls_hardclock_poll, NULL,
                                sizeof(unsigned long),
                                MBEDTLS_ENTROPY_SOURCE_WEAK);
    unsigned char serialNumber[SERIAL_NUMBER_LEN];
    (void) GetSerialNumber(serialNumber);   // Otherwise just use garbage from stack.
    int r = mbedtls_ctr_drbg_seed(&mCtrDrbg, mbedtls_entropy_func, &mEntropy, serialNumber, sizeof(serialNumber));
    if (r) {
        return r;
    }
    if (mSSLLoadRootCertificates) {
        // Root certificates can be large.
        // Allocate them in a separate heap.
        if (mRootCertificatesHeap == NULL) {
            (void) NewVMHeap(0, ROOT_CERTIFICATES_HEAP_SIZE, &mRootCertificatesHeap, 0);
        }
        Heap callerHeap = GetHeap();
        SetHeap(mRootCertificatesHeap);
        unwind_protect {
            r = ListRootCertificates(NULL, NULL);
            if (r < 0) {
                return r;
            }
            size_t listSize = r;
            char* list = (char*) ::NewPtr(listSize);
            r = ListRootCertificates(list, &listSize);
            if (r < 0) {
                ::DisposPtr(list);
                return r;
            }
            unsigned char* cert = (unsigned char*) ::NewPtr(0);
            size_t certPtrSize = 0;
            char* name = list;
            char* bufEnd = list + listSize;
            while (name < bufEnd) {
                size_t certSize = certPtrSize;
                r = GetRootCertificate(name, cert, &certSize);
                if (r >= 0) {
                    if (r > certSize) {
                        certPtrSize = r;
                        ::DisposPtr((Ptr) cert);
                        cert = (unsigned char*)::NewPtr(certPtrSize);
                        continue;
                    }
                    r = mbedtls_x509_crt_parse(&mSSLCAChain, cert, certSize);
                }
                name += strlen(name) + 1;
            }
            ::DisposPtr((Ptr) cert);
            ::DisposPtr((Ptr) list);
        } on_unwind {
            SetHeap(callerHeap);
        } end_unwind;
    }
    mbedtls_ssl_config_defaults(&mSSLConfig, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT);
    mbedtls_ssl_conf_rng(&mSSLConfig, mbedtls_ctr_drbg_random, &mCtrDrbg);
    mbedtls_ssl_conf_ca_chain(&mSSLConfig, &mSSLCAChain, NULL);
    mbedtls_ssl_conf_authmode(&mSSLConfig, mSSLAuthMode);
    mbedtls_ssl_setup(&mSSLContext, &mSSLConfig);
    mbedtls_ssl_set_bio(&mSSLContext, this, PMbedTLSConnectionEnd::SSLSendCallback, PMbedTLSConnectionEnd::SSLRecvCallback, NULL);
    mbedtls_ssl_set_timer_cb(&mSSLContext, this, PMbedTLSConnectionEnd::SSLSetTimerCallback, PMbedTLSConnectionEnd::SSLGetTimerCallback);
    if (mSSLHostname) {
        mbedtls_ssl_set_hostname(&mSSLContext, mSSLHostname);
        ::DisposPtr((Ptr) mSSLHostname);
        mSSLHostname = NULL;
    }
    mAsyncReadMessage = new TUAsyncMessage();
    mAsyncReadMessage->Init(false);
    mPutDataBuffer = New__11CBufferListFv();;
    mPutDataBuffer->Init(true);
    mSendBuffer = New__11CBufferListFv();;
    mSendBuffer->Init(true);
    mSendPending = false;
    mSendError = noErr;
    mGetDataBuffer = New__11CBufferListFv();
    mGetDataBuffer->Init(true);
    mSSLRecvBuffer = NULL;
    mSSLRecvBufferSize = 0;
    mRecvPending = false;
    mSSLReceiveInProgress = false;
    mRecvError = noErr;
    mSSLConfigured = true;
    mEstablished = false;
    mClosing = false;
    mPeerClosed = false;
    mSSLReadBuffer = NULL;
    mSSLReadBufferSize = 0;
    return 0;
}

// ------------------------------------------------------------------------ //
// * SSLUnconfigure(void)
// ------------------------------------------------------------------------ //
void
PMbedTLSConnectionEnd::SSLUnconfigure(void)
{
    if (mPutDataBuffer) {
        mPutDataBuffer->Delete();
        mPutDataBuffer = NULL;
    }
    if (mSendBuffer) {
        mSendBuffer->Delete();
        mSendBuffer = NULL;
    }
    if (mGetDataBuffer) {
        mGetDataBuffer->Delete();
        mGetDataBuffer = NULL;
    }
    if (mSSLRecvBuffer) {
        ::DisposPtr(mSSLRecvBuffer);
        mSSLRecvBuffer = NULL;
    }
    if (mSSLReadBuffer) {
        ::DisposPtr(mSSLReadBuffer);
        mSSLReadBuffer = NULL;
    }
    if (mAsyncReadMessage) {
        delete mAsyncReadMessage;
        mAsyncReadMessage = NULL;
    }
    if (mSSLHostname) {
        ::DisposPtr((Ptr) mSSLHostname);
        mSSLHostname = NULL;
    }
    mSSLConfigured = false;
}
