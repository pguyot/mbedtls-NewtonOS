// =========== Header ===========
// File:                PMbedTLSConnectionEnd.impl.h
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

#ifndef __PMBEDTLSCONNECTIONEND__
#define __PMBEDTLSCONNECTIONEND__

// NewtonOS

#include <PConnectionEnd.h>
#include <PCECallBack.h>

// mbedtls
#include "mbedtls/certs.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ssl_cache.h"
#include "mbedtls/version.h"
#include "mbedtls/x509.h"

#define kInetToolClassInfoNameRegisterFirstString "PInetToolCE"
#define kInetToolClassInfoNameRegisterSecondString "mbedTLS:Kallisys"
#define kInetToolClassInfoVersion    0

class PMbedTLSCECallBack;
class TCMOSSLCertificate;

///
/// Proxy for NIE PInetToolCE, patching calls to perform TLS/SSL.
///
PROTOCOL PMbedTLSConnectionEnd : public PConnectionEnd
    PROTOCOLVERSION(1.0)
{
public:
    PROTOCOL_IMPL_HEADER_MACRO( PMbedTLSConnectionEnd );

    ///
    /// Protocol constructor
    ///
    PConnectionEnd* New(void);

    ///
    /// Protocol desctructor.
    ///
    void            Delete (void);

                    // Allocate and intialize variables and do whatever else is necessary to be ready to handle requests
    NewtonErr       Init(PMuxTool* pmuxToolPtr, PCECallBack* ceCallBack, ULong ceNumber);

                    // This is the hook for associated PMuxTools.
                    // This may be used, for example, when a packet arrives for this connection end
    void            HandleInternalRequest(void* msgBuf);

                    // This method is called after returning from a request dispatch (e.g. putBytes).
                    // If the connection end needs to perform some operation but it can't be nested,
                    // you can set a flag and initiate the operation when HandleInternalEvent is called
    void            HandleInternalEvent();

    void            OpenStart();
    void            CloseStart();

    void            BindStart();
    void            UnbindStart();

    void            ConnectStart();
    void            ListenStart();
    void            AcceptStart();

    void            ReleaseStart();

    void            TerminateStart();

    void            PutBytesStart(CBufferList* clientBuffer);
    void            PutFramedBytesStart(CBufferList* clientBuffer, Boolean endOfFrame);
    void            KillPutStart();

    void            GetBytesStart(CBufferList* clientBuffer);
    void            GetFramedBytesStart(CBufferList* clientBuffer);
    void            GetBytesImmediateStart(CBufferList* clientBuffer, Size threshold);
    void            KillGetStart();

    void            HandleTimerTick();

    void            HandleRequest(UChar* request, ULong requestSize, TUMsgToken& msgToken, ULong msgType);
    void            HandleReply(ULong userRefCon, ULong msgType);

    void            KillControlStart();

    void            KillGetCommEventStart();

    void            GetCommEvent();

    ULong           ProcessOptionStart(TOption* theOption, ULong label, ULong opcode);  // 2.0
    ULong           ProcessPutBytesOptionStart(TOption* theOption, ULong label, ULong opcode);  // 2.0
    ULong           ProcessGetBytesOptionStart(TOption* theOption, ULong label, ULong opcode);  // 2.0

    void            ResArbReleaseStart(UChar* resName, UChar* resType); // 2.0
    void            ResArbClaimNotification(UChar* resName, UChar* resType);    // 2.0

    TUPort*         ForwardOptions();   // 2.0

private:
    /* PCECallBack callbacks, invoked through P-Class glue in PMbedTLSCECallBack.impl.s */
    void                    InitCB(TConnectionEnd* cePtr);


    void                    OpenCompleteCB(NewtonErr result);
    void                    CloseCompleteCB(NewtonErr result);

    void                    BindCompleteCB(NewtonErr result);
    void                    UnbindCompleteCB(NewtonErr result);

    void                    ConnectCompleteCB(NewtonErr result);
    void                    ListenCompleteCB(NewtonErr result);
    void                    AcceptCompleteCB(NewtonErr result);

    void                    TerminateCompleteCB();

    void                    PutCompleteCB(NewtonErr result, ULong putBytesCount);
    void                    KillPutCompleteCB(NewtonErr result);

    void                    GetCompleteCB(NewtonErr result, Boolean endOfFrame = false, ULong getBytesCount = 0);
    void                    KillGetCompleteCB(NewtonErr result);

    void                    HandleRequestCB(UChar* request, ULong requestSize, TUMsgToken& msgToken, ULong msgType);
    void                    HandleReplyCB(ULong userRefCon, ULong msgType);

    void                    KillControlCompleteCB(NewtonErr result);
    void                    KillGetCommEventCompleteCB(NewtonErr result);

    void                    GetCommEventCB();
    NewtonErr               PostCommEventCB(TCommToolGetEventReply& theEvent, NewtonErr result);

    void                    ProcessOptionCompleteCB(ULong optResult);
    void                    ProcessPutBytesOptionCompleteCB(ULong optResult);
    void                    ProcessGetBytesOptionCompleteCB(ULong optResult);

    void                    ResArbReleaseCompleteCB(NewtonErr result);

    NewtonErr               FlushChannelCB(ULong filter, NewtonErr flushResult);
    NewtonErr               StartAbortCB(NewtonErr abortError);
    NewtonErr               GetAbortErrCB();
    ULong                   GetTerminationEventCB();
    void                    SetTerminationEventCB(ULong event);

    NewtonErr               InitAsyncRPCMsgCB(TUAsyncMessage& asyncMsg, ULong refCon);

    // getters/settersÉ
    ULong                   GetToolConnectStateCB();
    void                    SetToolConnectStateCB(ULong state);

    TCMOCTConnectInfo&      GetConnectInfoCB();
    void                    SetConnectInfoCB(TCMOCTConnectInfo& info);

    TCMOTransportInfo&      GetCMOTransportInfoCB();
    void                    SetCMOTransportInfoCB(TCMOTransportInfo& info);

    ULong                   GetReceiveMessageBufSizeCB();   // returns the size of the buffer used by the commtool to receive rpc messages
                                                                           // Any message sent to the tool which exceeds this size will be truncated.
    TUPort*                 GetConnectionEndPortCB(); // returns a pointer to the tool's port object

    TCommToolOptionInfo&    GetControlOptionsInfoCB();  // returns the option info for the current control request
    TCommToolOptionInfo&    GetGetBytesOptionsInfoCB(); // returns the option info for the current get bytes request
    TCommToolOptionInfo&    GetPutBytesOptionsInfoCB(); // returns the option info for the current put bytes request

    CBufferList*            GetCurPutDataCB();          // returns a pointer to the current put request buffer list
    Boolean                 GetCurPutFrameDataCB();     // returns true if current put request is framed
    Boolean                 GetCurPutEndOfFrameCB();        // returns true if current framed put request is end of frame

    CBufferList*            GetCurGetDataCB();          // returns a pointer to the current get request buffer list
    Boolean                 GetCurGetFrameDataCB();     // returns true if current get request is framed
    Boolean                 GetCurGetNonBlockingCB();   // returns true if current get request is non blocking (ie has a threshold)
    Size                    GetCurGetThresholdCB();     // returns the value of threshold for the current nonblocking get request

    Boolean                 GetPassiveClaimCB();                        // returns true if client asked for passive claim of tool resources
    void                    SetPassiveClaimCB(Boolean passiveClaim);    // Set the value of passive claim

    Boolean                 GetPassiveStateCB();                        // returns true if in passive state (resources passively claimed, and willing to give up resources)
    void                    SetPassiveStateCB(Boolean passiveState);    // Set the value of passive claim

    Boolean                 GetWaitingForResNotifyCB();                     // returns true if tool has passively claimed resources, and is waiting for notification of resource ownership
    void                    SetWaitingForResNotifyCB(Boolean waitingForResNotify);  // Set the value of passive claim

    ULong                   GetCurRequestOpCodeCB();    // returns the opcode of the current control request

    static const ULong kAsyncReadMessageType = 0x00544C53;    // 'TLS'

    void DoSSLRead(CBufferList* clientBuffer, int threshold);
    void DoSSLHandshake();
    int SSLConfigure();
    void SSLUnconfigure();

	static int  SSLSendCallback(void *ctx, const unsigned char *buf, size_t len);
	static int  SSLRecvCallback(void *ctx, unsigned char *buf, size_t len);
	static void SSLSetTimerCallback(void *ctx, uint32_t int_ms, uint32_t fin_ms);
	static int  SSLGetTimerCallback(void *ctx);

    PMbedTLSCECallBack*     mThisCECallBack;
    TClassInfo*             mInetPConnectionEndClassInfo;
    PConnectionEnd*         mInetPConnectionEnd;
    PCECallBack*            mInetCECallBack;
    TUPort*                 mInetCEPort;
    TUAsyncMessage*         mAsyncReadMessage;
    Heap                    mRootCertificatesHeap;  // Our own heap with root certificates
    Boolean                 mSSLEnabled;    // from option
    Boolean                 mSSLConfigured; // means we should cleanup on close
    Boolean                 mSendPending;
    Boolean                 mRecvPending;
    Boolean                 mSSLReceiveInProgress;
    Boolean                 mEstablished;
    Boolean                 mUpgradable;
    Boolean                 mClosing;
    Boolean                 mPeerClosed;
    Boolean                 mSSLLoadRootCertificates;
    Boolean                 mSSLTimerFired;
    char*                   mSSLHostname;   // from option
    ULong                   mSSLAuthMode;   // from option
    mbedtls_entropy_context mEntropy;
    mbedtls_ctr_drbg_context    mCtrDrbg;
    mbedtls_x509_crt        mSSLCAChain;    // filled from option
    mbedtls_ssl_config      mSSLConfig;
    mbedtls_ssl_context     mSSLContext;
    /// Buffer we pass to NIE ConnectionEnd to send bytes
    CBufferList*            mPutDataBuffer;
    /// Buffer we pass to NIE ConnectionEnd to receive bytes
    CBufferList*            mGetDataBuffer;
    /// List of buffers to pass to NIE ConnectionEnd, filled by SSLSendCallback
    CBufferList*            mSendBuffer;        // buffer of bytes to send to TCP/IP
    /// Buffer we fill from NIE's GetCompleteCB
    Ptr                     mSSLRecvBuffer;
    int                     mSSLRecvBufferSize;
    /// Buffer we pass to mbedtls_ssl_read (must always be the same)
    Ptr                     mSSLReadBuffer;     // ssl read buffer
    int                     mSSLReadBufferSize; // current ssl read count
    int                     mSSLReadCount;      // current number of bytes we received
    NewtonErr               mSendError;
    NewtonErr               mRecvError;
    ULong                   mSSLTimerInt;
    ULong                   mSSLTimerFin;
};


#endif
    // __PMBEDTLSCONNECTIONEND__
