// =========== Header ===========
// File:                mbedTLSInterface.cpp
// Project:             mbed TLS for NewtonOS
// Written by:          Paul Guyot (pguyot@kallisys.net)
//
// Created on:          07/05/2020
// Internal version:    1
//
// Copyright:           © 2020 by Paul Guyot.
//                      All rights reserved worldwide.
// ===========

// =========== Change History ===========
// 07/05/2020   v1  [PG]    Creation of the file
// ===========

// NewtonOS
#include <NSandDDKIncludes.h>
#include <NewtonScript.h>
#include <NameServer.h>
#include <NewtonExceptions.h>
#include <OSErrors.h>
#include <NewtonErrors.h>

// Interface
#include "PMbedTLSInterface.h"

// ======================================================================== //
//  Prototypes
// ======================================================================== //
//
extern "C"
{
    Ref GetVersion(RefArg);
    Ref SSLListCiphersuites(RefArg);

    Ref EntropyNew(RefArg);

    Ref CtrDrbgNew(RefArg);
    Ref CtrDrbgSeed(RefArg, RefArg inCtrDrbgContextBin, RefArg inEntropyContextBin, RefArg inPers);

    Ref PKNew(RefArg);

    Ref SSLNew(RefArg);
    Ref SSLCheckPending(RefArg, RefArg inSSLBin);
    Ref SSLCloseNotify(RefArg, RefArg inSSLBin);
    Ref SSLGetALPNProtocol(RefArg, RefArg inSSLBin);
    Ref SSLGetBytesAvail(RefArg, RefArg inSSLBin);
    Ref SSLGetCiphersuite(RefArg, RefArg inSSLBin);
    Ref SSLGetMaxFragLen(RefArg, RefArg inSSLBin);
    Ref SSLGetMaxOutRecordPayload(RefArg, RefArg inSSLBin);
    Ref SSLGetPeerCert(RefArg, RefArg inSSLBin);
    Ref SSLGetRecordExpansion(RefArg, RefArg inSSLBin);
    Ref SSLGetSession(RefArg, RefArg inSSLBin);
    Ref SSLGetVerifyResult(RefArg, RefArg inSSLBin);
    Ref SSLGetVersion(RefArg, RefArg inSSLBin);
    Ref SSLHandshake(RefArg, RefArg inSSLBin);
    Ref SSLHandshakeStep(RefArg, RefArg inSSLBin);
    Ref SSLRead(RefArg, RefArg inSSLBin, RefArg inLen);
    Ref SSLRenegotiate(RefArg, RefArg inSSLBin);
    Ref SSLSendAlertMessage(RefArg, RefArg inSSLBin, RefArg inLevel, RefArg inMessage);
    Ref SSLSessionReset(RefArg, RefArg inSSLBin);
    Ref SSLSetBIO(RefArg, RefArg inSSLBin, RefArg inSendCb, RefArg inRecvCb);
    Ref SSLSetClientTransportID(RefArg, RefArg inSSLBin, RefArg inTransportLevelInfo);
    Ref SSLSetDatagramPacking(RefArg, RefArg inSSLBin, RefArg inAllowPacking);
    Ref SSLSetHostname(RefArg, RefArg inSSLBin, RefArg inHostname);
    Ref SSLSetHsAuthMode(RefArg, RefArg inSSLBin, RefArg inAuthmode);
    Ref SSLSetHsCAChain(RefArg, RefArg inSSLBin, RefArg inX509CrtBin, RefArg inX509CrlBin);
    Ref SSLSetHsOwnCert(RefArg, RefArg inSSLBin, RefArg inX509CrtBin, RefArg inPKBin);
    Ref SSLSetHsPSK(RefArg, RefArg inSSLBin, RefArg inPSKBin);
    Ref SSLSetMTU(RefArg, RefArg inSSLBin, RefArg inMTU);
    Ref SSLSetSession(RefArg, RefArg inSSLBin, RefArg inSession);
    Ref SSLSetTimerCb(RefArg, RefArg inSSLBin, RefArg inSetTimerCb, RefArg inGetTimerCb);
    Ref SSLSetup(RefArg, RefArg inSSLBin, RefArg inSSLConfigBin);
    Ref SSLWrite(RefArg, RefArg inSSLBin, RefArg inData);

    Ref SSLCacheNew(RefArg);
    Ref SSLCacheSetMaxEntries(RefArg, RefArg inSSLCacheBin, RefArg inMaxEntries);
    Ref SSLCacheSetTimeout(RefArg, RefArg inSSLCacheBin, RefArg inTimeout);

    Ref SSLConfigNew(RefArg);
    Ref SSLConfALPNProtocol(RefArg, RefArg inSSLConfigBin, RefArg inProtos);
    Ref SSLConfARC4Support(RefArg, RefArg inSSLConfigBin, RefArg inARC4);
    Ref SSLConfAuthMode(RefArg, RefArg inSSLConfigBin, RefArg inAuthMode);
    Ref SSLConfCAChain(RefArg, RefArg inSSLConfigBin, RefArg inX509CrtBin, RefArg inX509CrlBin);
    Ref SSLConfCbcRecordSplitting(RefArg, RefArg inSSLConfigBin, RefArg inSplit);
    Ref SSLConfCertProfile(RefArg, RefArg inSSLConfigBin, RefArg inCrtbin);
    Ref SSLConfCertReqCAList(RefArg, RefArg inSSLConfigBin, RefArg inCertReqCAList);
    Ref SSLConfCiphersuites(RefArg, RefArg inSSLConfigBin, RefArg inCiphersuites);
    Ref SSLConfCiphersuitesForVersion(RefArg, RefArg inSSLConfigBin, RefArg inCiphersuites, RefArg inMajor, RefArg inMinor);
    Ref SSLConfCurves(RefArg, RefArg inSSLConfigBin, RefArg inCurves);
    Ref SSLConfDbg(RefArg, RefArg inSSLConfigBin, RefArg inDebugCb);
    Ref SSLConfDHParamBin(RefArg, RefArg inSSLConfigBin, RefArg inDHMP, RefArg inDHMG);
    Ref SSLConfDHParamCtx(RefArg, RefArg inSSLConfigBin, RefArg inDHMContextBin);
    Ref SSLConfDHMMinBitlen(RefArg, RefArg inSSLConfigBin, RefArg inBitlen);
    Ref SSLConfDTLSAntiReplay(RefArg, RefArg inSSLConfigBin, RefArg inMode);
    Ref SSLConfDTLSBadMACLimit(RefArg, RefArg inSSLConfigBin, RefArg inLimit);
    Ref SSLConfDTLSCookies(RefArg, RefArg inSSLConfigBin, RefArg inCookiesWriteCb, RefArg inCookiesCheckCb);
    Ref SSLConfEncryptThenMac(RefArg, RefArg inSSLConfigBin, RefArg inETM);
    Ref SSLConfEndpoint(RefArg, RefArg inSSLConfigBin, RefArg inEndpoint);
    Ref SSLConfExportKeysCb(RefArg, RefArg inSSLConfigBin, RefArg inExportKeysCb);
    Ref SSLConfExtendedMasterSecret(RefArg, RefArg inSSLConfigBin, RefArg inEMS);
    Ref SSLConfFallback(RefArg, RefArg inSSLConfigBin, RefArg inFallback);
    Ref SSLConfHandshakeTimeout(RefArg, RefArg inSSLConfigBin, RefArg inMin, RefArg inMax);
    Ref SSLConfLegacyRenegotiation(RefArg, RefArg inSSLConfigBin, RefArg inAllowLegacy);
    Ref SSLConfMaxFragLen(RefArg, RefArg inSSLConfigBin, RefArg inMaxFragLen);
    Ref SSLConfMaxVersion(RefArg, RefArg inSSLConfigBin, RefArg inMajor, RefArg inMinor);
    Ref SSLConfMinVersion(RefArg, RefArg inSSLConfigBin, RefArg inMajor, RefArg inMinor);
    Ref SSLConfOwnCert(RefArg, RefArg inSSLConfigBin, RefArg inCrtBin, RefArg inPkBin);
    Ref SSLConfPSK(RefArg, RefArg inSSLConfigBin, RefArg inPSK, RefArg inPSKIdentity);
    Ref SSLConfPSKCb(RefArg, RefArg inSSLConfigBin, RefArg inPSKCb);
    Ref SSLConfRenegotiation(RefArg, RefArg inSSLConfigBin, RefArg inRenegotiation);
    Ref SSLConfRenegotiationEnforced(RefArg, RefArg inSSLConfigBin, RefArg inMaxRecords);
    Ref SSLConfRenegotiationPeriod(RefArg, RefArg inSSLConfigBin, RefArg inPeriod);
    Ref SSLConfRng(RefArg, RefArg inSSLConfigBin, RefArg inCtrDrbgContextBin);
    Ref SSLConfSessionCache(RefArg, RefArg inSSLConfigBin, RefArg inCacheBin);
    Ref SSLConfSessionTickets(RefArg, RefArg inSSLConfigBin, RefArg inUseTickets);
    Ref SSLConfSessionTicketsCb(RefArg, RefArg inSSLConfigBin, RefArg inTicketWriteCb, RefArg inTicketParseCb);
    Ref SSLConfSigHashes(RefArg, RefArg inSSLConfigBin, RefArg inSigHashes);
    Ref SSLConfSNI(RefArg, RefArg inSSLConfigBin, RefArg inSNICb);
    Ref SSLConfTransport(RefArg, RefArg inSSLConfigBin, RefArg inTransport);
    Ref SSLConfTruncatedHMAC(RefArg, RefArg inSSLConfigBin, RefArg inTruncate);
    Ref SSLConfVerify(RefArg, RefArg inSSLConfigBin, RefArg inVerifyCb);
    Ref SSLConfigDefaults(RefArg, RefArg inSSLConfigBin, RefArg inEndpoint, RefArg inTransport);

    Ref SSLSessionNew(RefArg);

    Ref X509CrlNew(RefArg);
    Ref X509CrlParse(RefArg, RefArg inX509CrlBin, RefArg inCRLBin);

    Ref X509CrtNew(RefArg);
    Ref X509CrtParse(RefArg, RefArg inX509CrtBin, RefArg inCertificateBin);
    Ref X509CrtCount(RefArg, RefArg inX509CrtBin);
    Ref X509CrtGetDER(RefArg, RefArg inX509CrtBin, RefArg inIndex);
    Ref X509CrtGetVersion(RefArg, RefArg inX509CrtBin, RefArg inIndex);
    Ref X509CrtGetSerialNumber(RefArg, RefArg inX509CrtBin, RefArg inIndex);
    Ref X509CrtGetIssuerName(RefArg, RefArg inX509CrtBin, RefArg inIndex);
    Ref X509CrtGetSubjectName(RefArg, RefArg inX509CrtBin, RefArg inIndex);
    Ref X509CrtGetIssueDate(RefArg, RefArg inX509CrtBin, RefArg inIndex);
    Ref X509CrtGetExpirationDate(RefArg, RefArg inX509CrtBin, RefArg inIndex);
    Ref X509CrtGetSignatureAlgorithm(RefArg, RefArg inX509CrtBin, RefArg inIndex);

    Ref Install(RefArg);
    Ref Remove(RefArg);

    Ref InstallConnectionEnd(RefArg);
    Ref RemoveConnectionEnd(RefArg);

    Ref GetRootCertificates(RefArg);
    Ref UnloadRootCertificates(RefArg);
}

// ------------------------------------------------------------------------ //
// * GetVersion( RefArg )
// ------------------------------------------------------------------------ //
// Return the version of the mbed TLS library in a NewtonScript Unicode string.
//
// The application can compare mbedtls_version_get_string and
// MBEDTLS_VERSION_STRING for consistency.
extern "C" Ref GetVersion( RefArg )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->GetVersion();
}

// ------------------------------------------------------------------------ //
// * SSLListCiphersuites( RefArg )
// ------------------------------------------------------------------------ //
// Return the list of supported cipher suites.
extern "C" Ref SSLListCiphersuites( RefArg )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLListCiphersuites();
}

// ------------------------------------------------------------------------ //
// * EntropyNew( RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref EntropyNew( RefArg )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->EntropyNew();
}


// ------------------------------------------------------------------------ //
// * CtrDrbgNew( RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref CtrDrbgNew( RefArg )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->CtrDrbgNew();
}

// ------------------------------------------------------------------------ //
// * CtrDrbgSeed( RefArg, RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref CtrDrbgSeed( RefArg, RefArg inCtrDrbgContextBin, RefArg inEntropyContextBin, RefArg inPers )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->CtrDrbgSeed(inCtrDrbgContextBin, inEntropyContextBin, inPers);
}

// ------------------------------------------------------------------------ //
// * PKNew( RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref PKNew( RefArg )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->PKNew();
}


// ------------------------------------------------------------------------ //
// * SSLNew( RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLNew( RefArg )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLNew();
}

// ------------------------------------------------------------------------ //
// * SSLCheckPending( RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLCheckPending( RefArg, RefArg inSSLBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLCheckPending(inSSLBin);
}

// ------------------------------------------------------------------------ //
// * SSLCloseNotify( RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLCloseNotify( RefArg, RefArg inSSLBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLCloseNotify(inSSLBin);
}

// ------------------------------------------------------------------------ //
// * SSLGetALPNProtocol( RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLGetALPNProtocol( RefArg, RefArg inSSLBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLGetALPNProtocol(inSSLBin);
}

// ------------------------------------------------------------------------ //
// * SSLGetBytesAvail( RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLGetBytesAvail( RefArg, RefArg inSSLBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLGetBytesAvail(inSSLBin);
}

// ------------------------------------------------------------------------ //
// * SSLGetCiphersuite( RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLGetCiphersuite( RefArg, RefArg inSSLBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLGetCiphersuite(inSSLBin);
}

// ------------------------------------------------------------------------ //
// * SSLGetMaxFragLen( RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLGetMaxFragLen( RefArg, RefArg inSSLBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLGetMaxFragLen(inSSLBin);
}

// ------------------------------------------------------------------------ //
// * SSLGetMaxOutRecordPayload( RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLGetMaxOutRecordPayload( RefArg, RefArg inSSLBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLGetMaxOutRecordPayload(inSSLBin);
}

// ------------------------------------------------------------------------ //
// * SSLGetPeerCert( RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLGetPeerCert( RefArg, RefArg inSSLBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLGetPeerCert(inSSLBin);
}

// ------------------------------------------------------------------------ //
// * SSLGetRecordExpansion( RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLGetRecordExpansion( RefArg, RefArg inSSLBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLGetRecordExpansion(inSSLBin);
}

// ------------------------------------------------------------------------ //
// * SSLGetSession( RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLGetSession( RefArg, RefArg inSSLBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLGetSession(inSSLBin);
}

// ------------------------------------------------------------------------ //
// * SSLGetVerifyResult( RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLGetVerifyResult( RefArg, RefArg inSSLBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLGetVerifyResult(inSSLBin);
}

// ------------------------------------------------------------------------ //
// * SSLGetVersion( RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLGetVersion( RefArg, RefArg inSSLBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLGetVersion(inSSLBin);
}

// ------------------------------------------------------------------------ //
// * SSLHandshake( RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLHandshake( RefArg, RefArg inSSLBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLHandshake(inSSLBin);
}

// ------------------------------------------------------------------------ //
// * SSLHandshakeStep( RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLHandshakeStep( RefArg, RefArg inSSLBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLHandshakeStep(inSSLBin);
}

// ------------------------------------------------------------------------ //
// * SSLRead( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLRead( RefArg, RefArg inSSLBin, RefArg inLen )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLRead(inSSLBin, inLen);
}

// ------------------------------------------------------------------------ //
// * SSLRenegotiate( RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLRenegotiate( RefArg, RefArg inSSLBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLRenegotiate(inSSLBin);
}

// ------------------------------------------------------------------------ //
// * SSLSendAlertMessage( RefArg, RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLSendAlertMessage( RefArg, RefArg inSSLBin, RefArg inLevel, RefArg inMessage )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLSendAlertMessage(inSSLBin, inLevel, inMessage);
}

// ------------------------------------------------------------------------ //
// * SSLSessionReset( RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLSessionReset( RefArg, RefArg inSSLBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLSessionReset(inSSLBin);
}

// ------------------------------------------------------------------------ //
// * SSLSetBIO( RefArg, RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLSetBIO( RefArg, RefArg inSSLBin, RefArg inSendCb, RefArg inRecvCb )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLSetBIO(inSSLBin, inSendCb, inRecvCb);
}

// ------------------------------------------------------------------------ //
// * SSLSetClientTransportID( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLSetClientTransportID( RefArg, RefArg inSSLBin, RefArg inTransportLevelInfo )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLSetClientTransportID(inSSLBin, inTransportLevelInfo);
}

// ------------------------------------------------------------------------ //
// * SSLSetDatagramPacking( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLSetDatagramPacking( RefArg, RefArg inSSLBin, RefArg inAllowPacking )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLSetDatagramPacking(inSSLBin, inAllowPacking);
}

// ------------------------------------------------------------------------ //
// * SSLSetHostname( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLSetHostname( RefArg, RefArg inSSLBin, RefArg inHostname )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLSetHostname(inSSLBin, inHostname);
}

// ------------------------------------------------------------------------ //
// * SSLSetHsAuthMode( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLSetHsAuthMode( RefArg, RefArg inSSLBin, RefArg inAuthmode )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLSetHsAuthMode(inSSLBin, inAuthmode);
}

// ------------------------------------------------------------------------ //
// * SSLSetHsCAChain( RefArg, RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLSetHsCAChain( RefArg, RefArg inSSLBin, RefArg inX509CrtBin, RefArg inX509CrlBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLSetHsCAChain(inSSLBin, inX509CrtBin, inX509CrlBin);
}

// ------------------------------------------------------------------------ //
// * SSLSetHsOwnCert( RefArg, RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLSetHsOwnCert( RefArg, RefArg inSSLBin, RefArg inX509CrtBin, RefArg inPKBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLSetHsOwnCert(inSSLBin, inX509CrtBin, inPKBin);
}

// ------------------------------------------------------------------------ //
// * SSLSetHsPSK( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLSetHsPSK( RefArg, RefArg inSSLBin, RefArg inPSKBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLSetHsPSK(inSSLBin, inPSKBin);
}

// ------------------------------------------------------------------------ //
// * SSLSetMTU( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLSetMTU( RefArg, RefArg inSSLBin, RefArg inMTU )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLSetMTU(inSSLBin, inMTU);
}

// ------------------------------------------------------------------------ //
// * SSLSetSession( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLSetSession( RefArg, RefArg inSSLBin, RefArg inSession )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLSetSession(inSSLBin, inSession);
}

// ------------------------------------------------------------------------ //
// * SSLSetTimerCb( RefArg, RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLSetTimerCb( RefArg, RefArg inSSLBin, RefArg inSetTimerCb, RefArg inGetTimerCb )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLSetTimerCb(inSSLBin, inSetTimerCb, inGetTimerCb);
}

// ------------------------------------------------------------------------ //
// * SSLSetup( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLSetup( RefArg, RefArg inSSLBin, RefArg inSSLConfigBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLSetup(inSSLBin, inSSLConfigBin);
}

// ------------------------------------------------------------------------ //
// * SSLWrite( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLWrite( RefArg, RefArg inSSLBin, RefArg inData )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLWrite(inSSLBin, inData);
}


// ------------------------------------------------------------------------ //
// * SSLCacheNew( RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLCacheNew( RefArg )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLCacheNew();
}

// ------------------------------------------------------------------------ //
// * SSLCacheSetMaxEntries( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLCacheSetMaxEntries( RefArg, RefArg inSSLCacheBin, RefArg inMaxEntries )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLCacheSetMaxEntries(inSSLCacheBin, inMaxEntries);
}

// ------------------------------------------------------------------------ //
// * SSLCacheSetTimeout( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLCacheSetTimeout( RefArg, RefArg inSSLCacheBin, RefArg inTimeout )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLCacheSetTimeout(inSSLCacheBin, inTimeout);
}


// ------------------------------------------------------------------------ //
// * SSLConfigNew( RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfigNew( RefArg )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfigNew();
}

// ------------------------------------------------------------------------ //
// * SSLConfALPNProtocol( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfALPNProtocol( RefArg, RefArg inSSLConfigBin, RefArg inProtos )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfALPNProtocol(inSSLConfigBin, inProtos);
}

// ------------------------------------------------------------------------ //
// * SSLConfARC4Support( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfARC4Support( RefArg, RefArg inSSLConfigBin, RefArg inARC4 )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfARC4Support(inSSLConfigBin, inARC4);
}

// ------------------------------------------------------------------------ //
// * SSLConfAuthMode( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfAuthMode( RefArg, RefArg inSSLConfigBin, RefArg inAuthMode )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfAuthMode(inSSLConfigBin, inAuthMode);
}

// ------------------------------------------------------------------------ //
// * SSLConfCAChain( RefArg, RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
// Configure CA chain.
extern "C" Ref SSLConfCAChain( RefArg, RefArg inSSLConfigBin, RefArg inX509CrtBin, RefArg inX509CrlBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfCAChain(inSSLConfigBin, inX509CrtBin, inX509CrlBin);
}

// ------------------------------------------------------------------------ //
// * SSLConfCbcRecordSplitting( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfCbcRecordSplitting( RefArg, RefArg inSSLConfigBin, RefArg inSplit )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfCbcRecordSplitting(inSSLConfigBin, inSplit);
}

// ------------------------------------------------------------------------ //
// * SSLConfCertProfile( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfCertProfile( RefArg, RefArg inSSLConfigBin, RefArg inCrtbin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfCertProfile(inSSLConfigBin, inCrtbin);
}

// ------------------------------------------------------------------------ //
// * SSLConfCertReqCAList( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfCertReqCAList( RefArg, RefArg inSSLConfigBin, RefArg inCertReqCAList )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfCertReqCAList(inSSLConfigBin, inCertReqCAList);
}

// ------------------------------------------------------------------------ //
// * SSLConfCiphersuites( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfCiphersuites( RefArg, RefArg inSSLConfigBin, RefArg inCiphersuites )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfCiphersuites(inSSLConfigBin, inCiphersuites);
}

// ------------------------------------------------------------------------ //
// * SSLConfCiphersuitesForVersion( RefArg, RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfCiphersuitesForVersion( RefArg, RefArg inSSLConfigBin, RefArg inCiphersuites, RefArg inMajor, RefArg inMinor )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfCiphersuitesForVersion(inSSLConfigBin, inCiphersuites, inMajor, inMinor);
}

// ------------------------------------------------------------------------ //
// * SSLConfCurves( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfCurves( RefArg, RefArg inSSLConfigBin, RefArg inCurves )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfCurves(inSSLConfigBin, inCurves);
}

// ------------------------------------------------------------------------ //
// * SSLConfDbg( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfDbg( RefArg, RefArg inSSLConfigBin, RefArg inDebugCb )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfDbg(inSSLConfigBin, inDebugCb);
}

// ------------------------------------------------------------------------ //
// * SSLConfDHParamBin( RefArg, RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfDHParamBin( RefArg, RefArg inSSLConfigBin, RefArg inDHMP, RefArg inDHMG )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfDHParamBin(inSSLConfigBin, inDHMP, inDHMG);
}

// ------------------------------------------------------------------------ //
// * SSLConfDHParamCtx( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfDHParamCtx( RefArg, RefArg inSSLConfigBin, RefArg inDHMContextBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfDHParamCtx(inSSLConfigBin, inDHMContextBin);
}

// ------------------------------------------------------------------------ //
// * SSLConfDHMMinBitlen( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfDHMMinBitlen( RefArg, RefArg inSSLConfigBin, RefArg inBitlen )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfDHMMinBitlen(inSSLConfigBin, inBitlen);
}

// ------------------------------------------------------------------------ //
// * SSLConfDTLSAntiReplay( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfDTLSAntiReplay( RefArg, RefArg inSSLConfigBin, RefArg inMode )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfDTLSAntiReplay(inSSLConfigBin, inMode);
}

// ------------------------------------------------------------------------ //
// * SSLConfDTLSBadMACLimit( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfDTLSBadMACLimit( RefArg, RefArg inSSLConfigBin, RefArg inLimit )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfDTLSBadMACLimit(inSSLConfigBin, inLimit);
}

// ------------------------------------------------------------------------ //
// * SSLConfDTLSCookies( RefArg, RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfDTLSCookies( RefArg, RefArg inSSLConfigBin, RefArg inCookiesWriteCb, RefArg inCookiesCheckCb )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfDTLSCookies(inSSLConfigBin, inCookiesWriteCb, inCookiesCheckCb);
}

// ------------------------------------------------------------------------ //
// * SSLConfEncryptThenMac( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfEncryptThenMac( RefArg, RefArg inSSLConfigBin, RefArg inETM )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfEncryptThenMac(inSSLConfigBin, inETM);
}

// ------------------------------------------------------------------------ //
// * SSLConfEndpoint( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfEndpoint( RefArg, RefArg inSSLConfigBin, RefArg inEndpoint )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfEndpoint(inSSLConfigBin, inEndpoint);
}

// ------------------------------------------------------------------------ //
// * SSLConfExportKeysCb( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfExportKeysCb( RefArg, RefArg inSSLConfigBin, RefArg inExportKeysCb )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfExportKeysCb(inSSLConfigBin, inExportKeysCb);
}

// ------------------------------------------------------------------------ //
// * SSLConfExtendedMasterSecret( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfExtendedMasterSecret( RefArg, RefArg inSSLConfigBin, RefArg inEMS )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfExtendedMasterSecret(inSSLConfigBin, inEMS);
}

// ------------------------------------------------------------------------ //
// * SSLConfFallback( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfFallback( RefArg, RefArg inSSLConfigBin, RefArg inFallback )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfFallback(inSSLConfigBin, inFallback);
}

// ------------------------------------------------------------------------ //
// * SSLConfHandshakeTimeout( RefArg, RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfHandshakeTimeout( RefArg, RefArg inSSLConfigBin, RefArg inMin, RefArg inMax )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfHandshakeTimeout(inSSLConfigBin, inMin, inMax);
}

// ------------------------------------------------------------------------ //
// * SSLConfLegacyRenegotiation( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfLegacyRenegotiation( RefArg, RefArg inSSLConfigBin, RefArg inAllowLegacy )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfLegacyRenegotiation(inSSLConfigBin, inAllowLegacy);
}

// ------------------------------------------------------------------------ //
// * SSLConfMaxFragLen( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfMaxFragLen( RefArg, RefArg inSSLConfigBin, RefArg inMaxFragLen )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfMaxFragLen(inSSLConfigBin, inMaxFragLen);
}

// ------------------------------------------------------------------------ //
// * SSLConfMaxVersion( RefArg, RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfMaxVersion( RefArg, RefArg inSSLConfigBin, RefArg inMajor, RefArg inMinor )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfMaxVersion(inSSLConfigBin, inMajor, inMinor);
}

// ------------------------------------------------------------------------ //
// * SSLConfMinVersion( RefArg, RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfMinVersion( RefArg, RefArg inSSLConfigBin, RefArg inMajor, RefArg inMinor )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfMinVersion(inSSLConfigBin, inMajor, inMinor);
}

// ------------------------------------------------------------------------ //
// * SSLConfOwnCert( RefArg, RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfOwnCert( RefArg, RefArg inSSLConfigBin, RefArg inCrtBin, RefArg inPkBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfOwnCert(inSSLConfigBin, inCrtBin, inPkBin);
}

// ------------------------------------------------------------------------ //
// * SSLConfPSK( RefArg, RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfPSK( RefArg, RefArg inSSLConfigBin, RefArg inPSK, RefArg inPSKIdentity )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfPSK(inSSLConfigBin, inPSK, inPSKIdentity);
}

// ------------------------------------------------------------------------ //
// * SSLConfPSKCb( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfPSKCb( RefArg, RefArg inSSLConfigBin, RefArg inPSKCb )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfPSKCb(inSSLConfigBin, inPSKCb);
}

// ------------------------------------------------------------------------ //
// * SSLConfRenegotiation( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfRenegotiation( RefArg, RefArg inSSLConfigBin, RefArg inRenegotiation )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfRenegotiation(inSSLConfigBin, inRenegotiation);
}

// ------------------------------------------------------------------------ //
// * SSLConfRenegotiationEnforced( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfRenegotiationEnforced( RefArg, RefArg inSSLConfigBin, RefArg inMaxRecords )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfRenegotiationEnforced(inSSLConfigBin, inMaxRecords);
}

// ------------------------------------------------------------------------ //
// * SSLConfRenegotiationPeriod( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfRenegotiationPeriod( RefArg, RefArg inSSLConfigBin, RefArg inPeriod )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfRenegotiationPeriod(inSSLConfigBin, inPeriod);
}

// ------------------------------------------------------------------------ //
// * SSLConfRng( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfRng( RefArg, RefArg inSSLConfigBin, RefArg inCtrDrbgContextBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfRng(inSSLConfigBin, inCtrDrbgContextBin);
}

// ------------------------------------------------------------------------ //
// * SSLConfSessionCache( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfSessionCache( RefArg, RefArg inSSLConfigBin, RefArg inCacheBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfSessionCache(inSSLConfigBin, inCacheBin);
}

// ------------------------------------------------------------------------ //
// * SSLConfSessionTickets( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfSessionTickets( RefArg, RefArg inSSLConfigBin, RefArg inUseTickets )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfSessionTickets(inSSLConfigBin, inUseTickets);
}

// ------------------------------------------------------------------------ //
// * SSLConfSessionTicketsCb( RefArg, RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfSessionTicketsCb( RefArg, RefArg inSSLConfigBin, RefArg inTicketWriteCb, RefArg inTicketParseCb )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfSessionTicketsCb(inSSLConfigBin, inTicketWriteCb, inTicketParseCb);
}

// ------------------------------------------------------------------------ //
// * SSLConfSigHashes( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfSigHashes( RefArg, RefArg inSSLConfigBin, RefArg inSigHashes )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfSigHashes(inSSLConfigBin, inSigHashes);
}

// ------------------------------------------------------------------------ //
// * SSLConfSNI( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfSNI( RefArg, RefArg inSSLConfigBin, RefArg inSNICb )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfSNI(inSSLConfigBin, inSNICb);
}

// ------------------------------------------------------------------------ //
// * SSLConfTransport( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfTransport( RefArg, RefArg inSSLConfigBin, RefArg inTransport )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfTransport(inSSLConfigBin, inTransport);
}

// ------------------------------------------------------------------------ //
// * SSLConfTruncatedHMAC( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfTruncatedHMAC( RefArg, RefArg inSSLConfigBin, RefArg inTruncate )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfTruncatedHMAC(inSSLConfigBin, inTruncate);
}

// ------------------------------------------------------------------------ //
// * SSLConfVerify( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfVerify( RefArg, RefArg inSSLConfigBin, RefArg inVerifyCb )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfVerify(inSSLConfigBin, inVerifyCb);
}

// ------------------------------------------------------------------------ //
// * SSLConfigDefaults( RefArg, RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLConfigDefaults( RefArg, RefArg inSSLConfigBin, RefArg inEndpoint, RefArg inTransport )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLConfigDefaults(inSSLConfigBin, inEndpoint, inTransport);
}


// ------------------------------------------------------------------------ //
// * SSLSessionNew( RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref SSLSessionNew( RefArg )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->SSLSessionNew();
}


// ------------------------------------------------------------------------ //
// * X509CrlNew( RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref X509CrlNew( RefArg )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->X509CrlNew();
}

// ------------------------------------------------------------------------ //
// * X509CrlParse( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref X509CrlParse( RefArg, RefArg inX509CrlBin, RefArg inCRLBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->X509CrlParse(inX509CrlBin, inCRLBin);
}


// ------------------------------------------------------------------------ //
// * X509CrtNew( RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref X509CrtNew( RefArg )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->X509CrtNew();
}

// ------------------------------------------------------------------------ //
// * X509CrtParse( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref X509CrtParse( RefArg, RefArg inX509CrtBin, RefArg inCertificateBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->X509CrtParse(inX509CrtBin, inCertificateBin);
}

// ------------------------------------------------------------------------ //
// * X509CrtCount( RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref X509CrtCount( RefArg, RefArg inX509CrtBin )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->X509CrtCount(inX509CrtBin);
}

// ------------------------------------------------------------------------ //
// * X509CrtGetDER( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref X509CrtGetDER( RefArg, RefArg inX509CrtBin, RefArg inIndex )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->X509CrtGetDER(inX509CrtBin, inIndex);
}

// ------------------------------------------------------------------------ //
// * X509CrtGetVersion( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref X509CrtGetVersion( RefArg, RefArg inX509CrtBin, RefArg inIndex )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->X509CrtGetVersion(inX509CrtBin, inIndex);
}

// ------------------------------------------------------------------------ //
// * X509CrtGetSerialNumber( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref X509CrtGetSerialNumber( RefArg, RefArg inX509CrtBin, RefArg inIndex )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->X509CrtGetSerialNumber(inX509CrtBin, inIndex);
}

// ------------------------------------------------------------------------ //
// * X509CrtGetIssuerName( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref X509CrtGetIssuerName( RefArg, RefArg inX509CrtBin, RefArg inIndex )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->X509CrtGetIssuerName(inX509CrtBin, inIndex);
}

// ------------------------------------------------------------------------ //
// * X509CrtGetSubjectName( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref X509CrtGetSubjectName( RefArg, RefArg inX509CrtBin, RefArg inIndex )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->X509CrtGetSubjectName(inX509CrtBin, inIndex);
}

// ------------------------------------------------------------------------ //
// * X509CrtGetIssueDate( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref X509CrtGetIssueDate( RefArg, RefArg inX509CrtBin, RefArg inIndex )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->X509CrtGetIssueDate(inX509CrtBin, inIndex);
}

// ------------------------------------------------------------------------ //
// * X509CrtGetExpirationDate( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref X509CrtGetExpirationDate( RefArg, RefArg inX509CrtBin, RefArg inIndex )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->X509CrtGetExpirationDate(inX509CrtBin, inIndex);
}

// ------------------------------------------------------------------------ //
// * X509CrtGetSignatureAlgorithm( RefArg, RefArg, RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref X509CrtGetSignatureAlgorithm( RefArg, RefArg inX509CrtBin, RefArg inIndex )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    return interface->X509CrtGetSignatureAlgorithm(inX509CrtBin, inIndex);
}

// ------------------------------------------------------------------------ //
// * Install( RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref Install( RefArg inRcvr )
{
    (void) PMbedTLSInterface::GetInterfaceSingleton();
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * Remove( RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref Remove( RefArg inRcvr )
{
    PMbedTLSInterface::DeleteInterfaceSingleton();
    return NILREF;
}

// ------------------------------------------------------------------------ //
// * InstallConnectionEnd( RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref InstallConnectionEnd( RefArg inRcvr )
{
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton();
    RefVar result = interface->InstallConnectionEnd();
    return result;
}

// ------------------------------------------------------------------------ //
// * RemoveConnectionEnd( RefArg )
// ------------------------------------------------------------------------ //
extern "C" Ref RemoveConnectionEnd( RefArg inRcvr )
{
    RefVar result = NILREF;
    PMbedTLSInterface* interface = PMbedTLSInterface::GetInterfaceSingleton(false);
    if (interface) {
        result = interface->RemoveConnectionEnd();
    }

    return result;
}

// ========================================== //
// Do not use the blue keys on this terminal. //
// ========================================== //
