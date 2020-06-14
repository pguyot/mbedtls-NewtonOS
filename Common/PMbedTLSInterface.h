// =========== Header ===========
// File:                PMbedTLSInterface.h
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

#ifndef __PMBEDTLSINTERFACE__
#define __PMBEDTLSINTERFACE__

#include <Protocols.h>

// Newton OS
#include <Newton.h>
#include <NSandDDKIncludes.h>

///
/// Glue protocol
///
PROTOCOL PMbedTLSInterface
    :
        public TProtocol
{
public:
    static  PMbedTLSInterface* GetInterfaceSingleton(bool create = true);
    static  void               DeleteInterfaceSingleton();

    ///
    /// Protocol constructor.
    ///
    static  PMbedTLSInterface*  New( char* inImplementationName );

    ///
    /// Protocol destructor.
    ///
    void            Delete();

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

    Ref X509CrlNew();
    Ref X509CrlParse(RefArg inX509CrlBin, RefArg inCRLBin);

	Ref InstallConnectionEnd();
	Ref RemoveConnectionEnd();
};

#endif
        // __PMBEDTLSINTERFACE__


// ======================================================== //
// The moving cursor writes, and having written, blinks on. //
// ======================================================== //
