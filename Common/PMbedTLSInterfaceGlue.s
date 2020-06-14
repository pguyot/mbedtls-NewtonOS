    .arch armv3
    .global _Z9NewByNamePKcS0_
    .global _Z19AllocInstanceByNamePKcS0_
    .global _Z12FreeInstanceP9TProtocol

    .macro _Dispatch slotno
    ldr     r0, [r0, #4]                // r0 -> "real" this
    ldr     r12, [r0, #8]               // r12 -> btable
    add     pc, r12, #\slotno*4         // go there
    .endm

    .macro New symbol, className, slotno=2
    .section ".text.\symbol\()", "ax"
    .global \symbol
    .balign 4
asciz__\className\():
    mov     r0, pc                      // r0 -> asciz name
    mov     pc, r14                     // begone!
    .string "\className\()"
    .balign 4
\symbol\():
    stmfd   r13!, {r0-r3, r14}          // save New(...) args
    mov     r1, r0                      // r1 = implementation name
    bl      asciz__\className           // r0 = interface name
    bl      _Z19AllocInstanceByNamePKcS0_ 
    tst     r0, r0                      // success?
    add     r13, r13, #4                // pop (saved) string parm; r0 is 'this' now (or nil)
    ldmfd   r13!, {r1-r3, r14}          // pop New(...) args
    moveq   pc, r14                     // return nil if instance couldn't be made
    _Dispatch   \slotno
    .endm

    .macro Delete symbol, slotno=3
    .section ".text.\symbol\()", "ax"
    .global \symbol
    .balign 4
\symbol\():
    ldr     r0, [r0, #4]                // r0 -> real this
    stmfd   r13!, {r0, r14}             // save this, LK
    adr     r14, return                 // set return addr
    ldr     r12, [r0, #8]               // r12 -> btable
    add     pc, r12, #\slotno*4         // call it
return:
    ldmfd   r13!, {r0, r14}             // restore this, return addr
    b       _Z12FreeInstanceP9TProtocol // tail call to free mem
    .endm

    .macro Dispatch symbol, slotno
    .section ".text.\symbol\()", "ax"
    .global \symbol
    .balign 4
\symbol\():
    _Dispatch    \slotno
    .endm

    New _ZN17PMbedTLSInterface3NewEPc, PMbedTLSInterface
    Delete _ZN17PMbedTLSInterface6DeleteEv

    Dispatch _ZN17PMbedTLSInterface10GetVersionEv, 4
    Dispatch _ZN17PMbedTLSInterface19SSLListCiphersuitesEv, 5

    Dispatch _ZN17PMbedTLSInterface10EntropyNewEv, 6

    Dispatch _ZN17PMbedTLSInterface10CtrDrbgNewEv, 7
    Dispatch _ZN17PMbedTLSInterface11CtrDrbgSeedERK6RefVarS2_S2_, 8

    Dispatch _ZN17PMbedTLSInterface5PKNewEv, 9

    Dispatch _ZN17PMbedTLSInterface6SSLNewEv, 10
    Dispatch _ZN17PMbedTLSInterface15SSLCheckPendingERK6RefVar, 11
    Dispatch _ZN17PMbedTLSInterface14SSLCloseNotifyERK6RefVar, 12
    Dispatch _ZN17PMbedTLSInterface18SSLGetALPNProtocolERK6RefVar, 13
    Dispatch _ZN17PMbedTLSInterface16SSLGetBytesAvailERK6RefVar, 14
    Dispatch _ZN17PMbedTLSInterface17SSLGetCiphersuiteERK6RefVar, 15
    Dispatch _ZN17PMbedTLSInterface16SSLGetMaxFragLenERK6RefVar, 16
    Dispatch _ZN17PMbedTLSInterface25SSLGetMaxOutRecordPayloadERK6RefVar, 17
    Dispatch _ZN17PMbedTLSInterface14SSLGetPeerCertERK6RefVar, 18
    Dispatch _ZN17PMbedTLSInterface21SSLGetRecordExpansionERK6RefVar, 19
    Dispatch _ZN17PMbedTLSInterface13SSLGetSessionERK6RefVar, 20
    Dispatch _ZN17PMbedTLSInterface18SSLGetVerifyResultERK6RefVar, 21
    Dispatch _ZN17PMbedTLSInterface13SSLGetVersionERK6RefVar, 22
    Dispatch _ZN17PMbedTLSInterface12SSLHandshakeERK6RefVar, 23
    Dispatch _ZN17PMbedTLSInterface16SSLHandshakeStepERK6RefVar, 24
    Dispatch _ZN17PMbedTLSInterface7SSLReadERK6RefVarS2_, 25
    Dispatch _ZN17PMbedTLSInterface14SSLRenegotiateERK6RefVar, 26
    Dispatch _ZN17PMbedTLSInterface19SSLSendAlertMessageERK6RefVarS2_S2_, 27
    Dispatch _ZN17PMbedTLSInterface15SSLSessionResetERK6RefVar, 28
    Dispatch _ZN17PMbedTLSInterface9SSLSetBIOERK6RefVarS2_S2_, 29
    Dispatch _ZN17PMbedTLSInterface23SSLSetClientTransportIDERK6RefVarS2_, 30
    Dispatch _ZN17PMbedTLSInterface21SSLSetDatagramPackingERK6RefVarS2_, 31
    Dispatch _ZN17PMbedTLSInterface14SSLSetHostnameERK6RefVarS2_, 32
    Dispatch _ZN17PMbedTLSInterface16SSLSetHsAuthModeERK6RefVarS2_, 33
    Dispatch _ZN17PMbedTLSInterface15SSLSetHsCAChainERK6RefVarS2_S2_, 34
    Dispatch _ZN17PMbedTLSInterface15SSLSetHsOwnCertERK6RefVarS2_S2_, 35
    Dispatch _ZN17PMbedTLSInterface11SSLSetHsPSKERK6RefVarS2_, 36
    Dispatch _ZN17PMbedTLSInterface9SSLSetMTUERK6RefVarS2_, 37
    Dispatch _ZN17PMbedTLSInterface13SSLSetSessionERK6RefVarS2_, 38
    Dispatch _ZN17PMbedTLSInterface13SSLSetTimerCbERK6RefVarS2_S2_, 39
    Dispatch _ZN17PMbedTLSInterface8SSLSetupERK6RefVarS2_, 40
    Dispatch _ZN17PMbedTLSInterface8SSLWriteERK6RefVarS2_, 41

    Dispatch _ZN17PMbedTLSInterface11SSLCacheNewEv, 42
    Dispatch _ZN17PMbedTLSInterface21SSLCacheSetMaxEntriesERK6RefVarS2_, 43
    Dispatch _ZN17PMbedTLSInterface18SSLCacheSetTimeoutERK6RefVarS2_, 44

    Dispatch _ZN17PMbedTLSInterface12SSLConfigNewEv, 45
    Dispatch _ZN17PMbedTLSInterface19SSLConfALPNProtocolERK6RefVarS2_, 46
    Dispatch _ZN17PMbedTLSInterface18SSLConfARC4SupportERK6RefVarS2_, 47
    Dispatch _ZN17PMbedTLSInterface15SSLConfAuthModeERK6RefVarS2_, 48
    Dispatch _ZN17PMbedTLSInterface14SSLConfCAChainERK6RefVarS2_S2_, 49
    Dispatch _ZN17PMbedTLSInterface25SSLConfCbcRecordSplittingERK6RefVarS2_, 50
    Dispatch _ZN17PMbedTLSInterface18SSLConfCertProfileERK6RefVarS2_, 51
    Dispatch _ZN17PMbedTLSInterface20SSLConfCertReqCAListERK6RefVarS2_, 52
    Dispatch _ZN17PMbedTLSInterface19SSLConfCiphersuitesERK6RefVarS2_, 53
    Dispatch _ZN17PMbedTLSInterface29SSLConfCiphersuitesForVersionERK6RefVarS2_S2_S2_, 54
    Dispatch _ZN17PMbedTLSInterface13SSLConfCurvesERK6RefVarS2_, 55
    Dispatch _ZN17PMbedTLSInterface10SSLConfDbgERK6RefVarS2_, 56
    Dispatch _ZN17PMbedTLSInterface17SSLConfDHParamBinERK6RefVarS2_S2_, 57
    Dispatch _ZN17PMbedTLSInterface17SSLConfDHParamCtxERK6RefVarS2_, 58
    Dispatch _ZN17PMbedTLSInterface19SSLConfDHMMinBitlenERK6RefVarS2_, 59
    Dispatch _ZN17PMbedTLSInterface21SSLConfDTLSAntiReplayERK6RefVarS2_, 60
    Dispatch _ZN17PMbedTLSInterface22SSLConfDTLSBadMACLimitERK6RefVarS2_, 61
    Dispatch _ZN17PMbedTLSInterface18SSLConfDTLSCookiesERK6RefVarS2_S2_, 62
    Dispatch _ZN17PMbedTLSInterface21SSLConfEncryptThenMacERK6RefVarS2_, 63
    Dispatch _ZN17PMbedTLSInterface15SSLConfEndpointERK6RefVarS2_, 64
    Dispatch _ZN17PMbedTLSInterface19SSLConfExportKeysCbERK6RefVarS2_, 65
    Dispatch _ZN17PMbedTLSInterface27SSLConfExtendedMasterSecretERK6RefVarS2_, 66
    Dispatch _ZN17PMbedTLSInterface15SSLConfFallbackERK6RefVarS2_, 67
    Dispatch _ZN17PMbedTLSInterface23SSLConfHandshakeTimeoutERK6RefVarS2_S2_, 68
    Dispatch _ZN17PMbedTLSInterface26SSLConfLegacyRenegotiationERK6RefVarS2_, 69
    Dispatch _ZN17PMbedTLSInterface17SSLConfMaxFragLenERK6RefVarS2_, 70
    Dispatch _ZN17PMbedTLSInterface17SSLConfMaxVersionERK6RefVarS2_S2_, 71
    Dispatch _ZN17PMbedTLSInterface17SSLConfMinVersionERK6RefVarS2_S2_, 72
    Dispatch _ZN17PMbedTLSInterface14SSLConfOwnCertERK6RefVarS2_S2_, 73
    Dispatch _ZN17PMbedTLSInterface10SSLConfPSKERK6RefVarS2_S2_, 74
    Dispatch _ZN17PMbedTLSInterface12SSLConfPSKCbERK6RefVarS2_, 75
    Dispatch _ZN17PMbedTLSInterface20SSLConfRenegotiationERK6RefVarS2_, 76
    Dispatch _ZN17PMbedTLSInterface28SSLConfRenegotiationEnforcedERK6RefVarS2_, 77
    Dispatch _ZN17PMbedTLSInterface26SSLConfRenegotiationPeriodERK6RefVarS2_, 78
    Dispatch _ZN17PMbedTLSInterface10SSLConfRngERK6RefVarS2_, 79
    Dispatch _ZN17PMbedTLSInterface19SSLConfSessionCacheERK6RefVarS2_, 80
    Dispatch _ZN17PMbedTLSInterface21SSLConfSessionTicketsERK6RefVarS2_, 81
    Dispatch _ZN17PMbedTLSInterface23SSLConfSessionTicketsCbERK6RefVarS2_S2_, 82
    Dispatch _ZN17PMbedTLSInterface16SSLConfSigHashesERK6RefVarS2_, 83
    Dispatch _ZN17PMbedTLSInterface10SSLConfSNIERK6RefVarS2_, 84
    Dispatch _ZN17PMbedTLSInterface16SSLConfTransportERK6RefVarS2_, 85
    Dispatch _ZN17PMbedTLSInterface20SSLConfTruncatedHMACERK6RefVarS2_, 86
    Dispatch _ZN17PMbedTLSInterface13SSLConfVerifyERK6RefVarS2_, 87
    Dispatch _ZN17PMbedTLSInterface17SSLConfigDefaultsERK6RefVarS2_S2_, 88

    Dispatch _ZN17PMbedTLSInterface13SSLSessionNewEv, 89

    Dispatch _ZN17PMbedTLSInterface10X509CrlNewEv, 90
    Dispatch _ZN17PMbedTLSInterface12X509CrlParseERK6RefVarS2_, 91

    Dispatch _ZN17PMbedTLSInterface10X509CrtNewEv, 92
    Dispatch _ZN17PMbedTLSInterface12X509CrtParseERK6RefVarS2_, 93
    Dispatch _ZN17PMbedTLSInterface12X509CrtCountERK6RefVar, 94
    Dispatch _ZN17PMbedTLSInterface13X509CrtGetDERERK6RefVarS2_, 95
    Dispatch _ZN17PMbedTLSInterface17X509CrtGetVersionERK6RefVarS2_, 96
    Dispatch _ZN17PMbedTLSInterface22X509CrtGetSerialNumberERK6RefVarS2_, 97
    Dispatch _ZN17PMbedTLSInterface20X509CrtGetIssuerNameERK6RefVarS2_, 98
    Dispatch _ZN17PMbedTLSInterface21X509CrtGetSubjectNameERK6RefVarS2_, 99
    Dispatch _ZN17PMbedTLSInterface19X509CrtGetIssueDateERK6RefVarS2_, 100
    Dispatch _ZN17PMbedTLSInterface24X509CrtGetExpirationDateERK6RefVarS2_, 101
    Dispatch _ZN17PMbedTLSInterface28X509CrtGetSignatureAlgorithmERK6RefVarS2_, 102

    Dispatch _ZN17PMbedTLSInterface20InstallConnectionEndEv, 103
    Dispatch _ZN17PMbedTLSInterface19RemoveConnectionEndEv, 104
