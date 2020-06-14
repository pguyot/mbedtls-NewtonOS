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

    New _ZN11PCECallBack3NewEPc, PCECallBack
    Delete _ZN11PCECallBack6DeleteEv

    Dispatch _ZN11PCECallBack4InitEP14TConnectionEnd, 4
    Dispatch _ZN11PCECallBack12OpenCompleteEl, 5
    Dispatch _ZN11PCECallBack13CloseCompleteEl, 6
    Dispatch _ZN11PCECallBack12BindCompleteEl, 7
    Dispatch _ZN11PCECallBack14UnbindCompleteEl, 8
    Dispatch _ZN11PCECallBack15ConnectCompleteEl, 9
    Dispatch _ZN11PCECallBack14ListenCompleteEl, 10
    Dispatch _ZN11PCECallBack14AcceptCompleteEl, 11
    Dispatch _ZN11PCECallBack17TerminateCompleteEv, 12
    Dispatch _ZN11PCECallBack11PutCompleteElm, 13
    Dispatch _ZN11PCECallBack15KillPutCompleteEl, 14
    Dispatch _ZN11PCECallBack11GetCompleteElhm, 15
    Dispatch _ZN11PCECallBack15KillGetCompleteEl, 16
    Dispatch _ZN11PCECallBack13HandleRequestEPhmR10TUMsgTokenm, 17
    Dispatch _ZN11PCECallBack11HandleReplyEmm, 18
    Dispatch _ZN11PCECallBack19KillControlCompleteEl, 19
    Dispatch _ZN11PCECallBack24KillGetCommEventCompleteEl, 20
    Dispatch _ZN11PCECallBack12GetCommEventEv, 21
    Dispatch _ZN11PCECallBack13PostCommEventER22TCommToolGetEventReplyl, 22
    Dispatch _ZN11PCECallBack21ProcessOptionCompleteEm, 23
    Dispatch _ZN11PCECallBack29ProcessPutBytesOptionCompleteEm, 24
    Dispatch _ZN11PCECallBack29ProcessGetBytesOptionCompleteEm, 25
    Dispatch _ZN11PCECallBack21ResArbReleaseCompleteEl, 26
    Dispatch _ZN11PCECallBack12FlushChannelEml, 27
    Dispatch _ZN11PCECallBack10StartAbortEl, 28
    Dispatch _ZN11PCECallBack11GetAbortErrEv, 29
    Dispatch _ZN11PCECallBack19GetTerminationEventEv, 30
    Dispatch _ZN11PCECallBack19SetTerminationEventEm, 31
    Dispatch _ZN11PCECallBack15InitAsyncRPCMsgER14TUAsyncMessagem, 32
    Dispatch _ZN11PCECallBack19GetToolConnectStateEv, 33
    Dispatch _ZN11PCECallBack19SetToolConnectStateEm, 34
    Dispatch _ZN11PCECallBack14GetConnectInfoEv, 35
    Dispatch _ZN11PCECallBack14SetConnectInfoER17TCMOCTConnectInfo, 36
    Dispatch _ZN11PCECallBack19GetCMOTransportInfoEv, 37
    Dispatch _ZN11PCECallBack19SetCMOTransportInfoER17TCMOTransportInfo, 38
    Dispatch _ZN11PCECallBack24GetReceiveMessageBufSizeEv, 39
    Dispatch _ZN11PCECallBack20GetConnectionEndPortEv, 40
    Dispatch _ZN11PCECallBack21GetControlOptionsInfoEv, 41
    Dispatch _ZN11PCECallBack22GetGetBytesOptionsInfoEv, 42
    Dispatch _ZN11PCECallBack22GetPutBytesOptionsInfoEv, 43
    Dispatch _ZN11PCECallBack13GetCurPutDataEv, 44
    Dispatch _ZN11PCECallBack18GetCurPutFrameDataEv, 45
    Dispatch _ZN11PCECallBack19GetCurPutEndOfFrameEv, 46
    Dispatch _ZN11PCECallBack13GetCurGetDataEv, 47
    Dispatch _ZN11PCECallBack18GetCurGetFrameDataEv, 48
    Dispatch _ZN11PCECallBack20GetCurGetNonBlockingEv, 49
    Dispatch _ZN11PCECallBack18GetCurGetThresholdEv, 50
    Dispatch _ZN11PCECallBack15GetPassiveClaimEv, 51
    Dispatch _ZN11PCECallBack15SetPassiveClaimEh, 52
    Dispatch _ZN11PCECallBack15GetPassiveStateEv, 53
    Dispatch _ZN11PCECallBack15SetPassiveStateEh, 54
    Dispatch _ZN11PCECallBack22GetWaitingForResNotifyEv, 55
    Dispatch _ZN11PCECallBack22SetWaitingForResNotifyEh, 56
    Dispatch _ZN11PCECallBack19GetCurRequestOpCodeEv, 57
