    .arch armv3
    .section .interp, "ax"
    . = 0
    .balign 4

    .global main

main:
local___ZN8PMbedTLS9ClassInfoEv:
@ (reserved for now)
    .word   0
    .word   __implname__ - .
    .word   __protname__ - .
    .word   __cap__ - .
    .word   __btbl__ - .
    .word   __monent__ - .
@ branch to sizeof glue
    b       _ZN8PMbedTLS6SizeofEv
@ zero, or branch to Alloc method
    .word   0
@ zero, or branch to Free method
    .word   0
@ return, or branch to New(void)
    b       _ZN8PMbedTLS3NewEv
@ return, or branch to Delete
    b       _ZN8PMbedTLS6DeleteEv
@ version number
    .word   0x00010000
@ flags
    .word   0x00000000
@ (reserved)
    .word   0x00000000
@ (reserved)
    b       return__nil__8PMbedTLS

_ZN8PMbedTLS9ClassInfoEv:
    adr     r0,local___ZN8PMbedTLS9ClassInfoEv
    mov     pc,r14

return__nil__8PMbedTLS:
    mov     r0,#0
    mov     pc,r14

__implname__:
    .string "PMbedTLS"
__protname__:
    .string "PMbedTLSInterface"
    .balign 4
__cap__:
    .word   0
__btbl__:
    .word   0
    b   _ZN8PMbedTLS9ClassInfoEv
    b   _ZN8PMbedTLS3NewEv
    b   _ZN8PMbedTLS6DeleteEv

    b   _ZN8PMbedTLS10GetVersionEv
    b   _ZN8PMbedTLS19SSLListCiphersuitesEv

    b   _ZN8PMbedTLS10EntropyNewEv

    b   _ZN8PMbedTLS10CtrDrbgNewEv
    b   _ZN8PMbedTLS11CtrDrbgSeedERK6RefVarS2_S2_

    b   _ZN8PMbedTLS5PKNewEv

    b   _ZN8PMbedTLS6SSLNewEv
    b   _ZN8PMbedTLS15SSLCheckPendingERK6RefVar
    b   _ZN8PMbedTLS14SSLCloseNotifyERK6RefVar
    b   _ZN8PMbedTLS18SSLGetALPNProtocolERK6RefVar
    b   _ZN8PMbedTLS16SSLGetBytesAvailERK6RefVar
    b   _ZN8PMbedTLS17SSLGetCiphersuiteERK6RefVar
    b   _ZN8PMbedTLS16SSLGetMaxFragLenERK6RefVar
    b   _ZN8PMbedTLS25SSLGetMaxOutRecordPayloadERK6RefVar
    b   _ZN8PMbedTLS14SSLGetPeerCertERK6RefVar
    b   _ZN8PMbedTLS21SSLGetRecordExpansionERK6RefVar
    b   _ZN8PMbedTLS13SSLGetSessionERK6RefVar
    b   _ZN8PMbedTLS18SSLGetVerifyResultERK6RefVar
    b   _ZN8PMbedTLS13SSLGetVersionERK6RefVar
    b   _ZN8PMbedTLS12SSLHandshakeERK6RefVar
    b   _ZN8PMbedTLS16SSLHandshakeStepERK6RefVar
    b   _ZN8PMbedTLS7SSLReadERK6RefVarS2_
    b   _ZN8PMbedTLS14SSLRenegotiateERK6RefVar
    b   _ZN8PMbedTLS19SSLSendAlertMessageERK6RefVarS2_S2_
    b   _ZN8PMbedTLS15SSLSessionResetERK6RefVar
    b   _ZN8PMbedTLS9SSLSetBIOERK6RefVarS2_S2_
    b   _ZN8PMbedTLS23SSLSetClientTransportIDERK6RefVarS2_
    b   _ZN8PMbedTLS21SSLSetDatagramPackingERK6RefVarS2_
    b   _ZN8PMbedTLS14SSLSetHostnameERK6RefVarS2_
    b   _ZN8PMbedTLS16SSLSetHsAuthModeERK6RefVarS2_
    b   _ZN8PMbedTLS15SSLSetHsCAChainERK6RefVarS2_S2_
    b   _ZN8PMbedTLS15SSLSetHsOwnCertERK6RefVarS2_S2_
    b   _ZN8PMbedTLS11SSLSetHsPSKERK6RefVarS2_
    b   _ZN8PMbedTLS9SSLSetMTUERK6RefVarS2_
    b   _ZN8PMbedTLS13SSLSetSessionERK6RefVarS2_
    b   _ZN8PMbedTLS13SSLSetTimerCbERK6RefVarS2_S2_
    b   _ZN8PMbedTLS8SSLSetupERK6RefVarS2_
    b   _ZN8PMbedTLS8SSLWriteERK6RefVarS2_

    b   _ZN8PMbedTLS11SSLCacheNewEv
    b   _ZN8PMbedTLS21SSLCacheSetMaxEntriesERK6RefVarS2_
    b   _ZN8PMbedTLS18SSLCacheSetTimeoutERK6RefVarS2_

    b   _ZN8PMbedTLS12SSLConfigNewEv
    b   _ZN8PMbedTLS19SSLConfALPNProtocolERK6RefVarS2_
    b   _ZN8PMbedTLS18SSLConfARC4SupportERK6RefVarS2_
    b   _ZN8PMbedTLS15SSLConfAuthModeERK6RefVarS2_
    b   _ZN8PMbedTLS14SSLConfCAChainERK6RefVarS2_S2_
    b   _ZN8PMbedTLS25SSLConfCbcRecordSplittingERK6RefVarS2_
    b   _ZN8PMbedTLS18SSLConfCertProfileERK6RefVarS2_
    b   _ZN8PMbedTLS20SSLConfCertReqCAListERK6RefVarS2_
    b   _ZN8PMbedTLS19SSLConfCiphersuitesERK6RefVarS2_
    b   _ZN8PMbedTLS29SSLConfCiphersuitesForVersionERK6RefVarS2_S2_S2_
    b   _ZN8PMbedTLS13SSLConfCurvesERK6RefVarS2_
    b   _ZN8PMbedTLS10SSLConfDbgERK6RefVarS2_
    b   _ZN8PMbedTLS17SSLConfDHParamBinERK6RefVarS2_S2_
    b   _ZN8PMbedTLS17SSLConfDHParamCtxERK6RefVarS2_
    b   _ZN8PMbedTLS19SSLConfDHMMinBitlenERK6RefVarS2_
    b   _ZN8PMbedTLS21SSLConfDTLSAntiReplayERK6RefVarS2_
    b   _ZN8PMbedTLS22SSLConfDTLSBadMACLimitERK6RefVarS2_
    b   _ZN8PMbedTLS18SSLConfDTLSCookiesERK6RefVarS2_S2_
    b   _ZN8PMbedTLS21SSLConfEncryptThenMacERK6RefVarS2_
    b   _ZN8PMbedTLS15SSLConfEndpointERK6RefVarS2_
    b   _ZN8PMbedTLS19SSLConfExportKeysCbERK6RefVarS2_
    b   _ZN8PMbedTLS27SSLConfExtendedMasterSecretERK6RefVarS2_
    b   _ZN8PMbedTLS15SSLConfFallbackERK6RefVarS2_
    b   _ZN8PMbedTLS23SSLConfHandshakeTimeoutERK6RefVarS2_S2_
    b   _ZN8PMbedTLS26SSLConfLegacyRenegotiationERK6RefVarS2_
    b   _ZN8PMbedTLS17SSLConfMaxFragLenERK6RefVarS2_
    b   _ZN8PMbedTLS17SSLConfMaxVersionERK6RefVarS2_S2_
    b   _ZN8PMbedTLS17SSLConfMinVersionERK6RefVarS2_S2_
    b   _ZN8PMbedTLS14SSLConfOwnCertERK6RefVarS2_S2_
    b   _ZN8PMbedTLS10SSLConfPSKERK6RefVarS2_S2_
    b   _ZN8PMbedTLS12SSLConfPSKCbERK6RefVarS2_
    b   _ZN8PMbedTLS20SSLConfRenegotiationERK6RefVarS2_
    b   _ZN8PMbedTLS28SSLConfRenegotiationEnforcedERK6RefVarS2_
    b   _ZN8PMbedTLS26SSLConfRenegotiationPeriodERK6RefVarS2_
    b   _ZN8PMbedTLS10SSLConfRngERK6RefVarS2_
    b   _ZN8PMbedTLS19SSLConfSessionCacheERK6RefVarS2_
    b   _ZN8PMbedTLS21SSLConfSessionTicketsERK6RefVarS2_
    b   _ZN8PMbedTLS23SSLConfSessionTicketsCbERK6RefVarS2_S2_
    b   _ZN8PMbedTLS16SSLConfSigHashesERK6RefVarS2_
    b   _ZN8PMbedTLS10SSLConfSNIERK6RefVarS2_
    b   _ZN8PMbedTLS16SSLConfTransportERK6RefVarS2_
    b   _ZN8PMbedTLS20SSLConfTruncatedHMACERK6RefVarS2_
    b   _ZN8PMbedTLS13SSLConfVerifyERK6RefVarS2_
    b   _ZN8PMbedTLS17SSLConfigDefaultsERK6RefVarS2_S2_

    b   _ZN8PMbedTLS13SSLSessionNewEv

    b   _ZN8PMbedTLS10X509CrlNewEv
    b   _ZN8PMbedTLS12X509CrlParseERK6RefVarS2_

    b   _ZN8PMbedTLS10X509CrtNewEv
    b   _ZN8PMbedTLS12X509CrtParseERK6RefVarS2_
    b   _ZN8PMbedTLS12X509CrtCountERK6RefVar
    b   _ZN8PMbedTLS13X509CrtGetDERERK6RefVarS2_
    b   _ZN8PMbedTLS17X509CrtGetVersionERK6RefVarS2_
    b   _ZN8PMbedTLS22X509CrtGetSerialNumberERK6RefVarS2_
    b   _ZN8PMbedTLS20X509CrtGetIssuerNameERK6RefVarS2_
    b   _ZN8PMbedTLS21X509CrtGetSubjectNameERK6RefVarS2_
    b   _ZN8PMbedTLS19X509CrtGetIssueDateERK6RefVarS2_
    b   _ZN8PMbedTLS24X509CrtGetExpirationDateERK6RefVarS2_
    b   _ZN8PMbedTLS28X509CrtGetSignatureAlgorithmERK6RefVarS2_

    b   _ZN8PMbedTLS20InstallConnectionEndEv
    b   _ZN8PMbedTLS19RemoveConnectionEndEv

__monent__:
    .end
