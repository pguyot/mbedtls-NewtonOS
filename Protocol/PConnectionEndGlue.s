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

    New _ZN14PConnectionEnd3NewEPc, PConnectionEnd
    Delete _ZN14PConnectionEnd6DeleteEv

    Dispatch _ZN14PConnectionEnd4InitEP8PMuxToolP11PCECallBackm, 4
    Dispatch _ZN14PConnectionEnd21HandleInternalRequestEPv, 5
    Dispatch _ZN14PConnectionEnd19HandleInternalEventEv, 6
    Dispatch _ZN14PConnectionEnd9OpenStartEv, 7
    Dispatch _ZN14PConnectionEnd10CloseStartEv, 8
    Dispatch _ZN14PConnectionEnd9BindStartEv, 9
    Dispatch _ZN14PConnectionEnd11UnbindStartEv, 10
    Dispatch _ZN14PConnectionEnd12ConnectStartEv, 11
    Dispatch _ZN14PConnectionEnd11ListenStartEv, 12
    Dispatch _ZN14PConnectionEnd11AcceptStartEv, 13
    Dispatch _ZN14PConnectionEnd12ReleaseStartEv, 14
    Dispatch _ZN14PConnectionEnd14TerminateStartEv, 15
    Dispatch _ZN14PConnectionEnd13PutBytesStartEP11CBufferList, 16
    Dispatch _ZN14PConnectionEnd19PutFramedBytesStartEP11CBufferListh, 17
    Dispatch _ZN14PConnectionEnd12KillPutStartEv, 18
    Dispatch _ZN14PConnectionEnd13GetBytesStartEP11CBufferList, 19
    Dispatch _ZN14PConnectionEnd19GetFramedBytesStartEP11CBufferList, 20
    Dispatch _ZN14PConnectionEnd22GetBytesImmediateStartEP11CBufferListl, 21
    Dispatch _ZN14PConnectionEnd12KillGetStartEv, 22
    Dispatch _ZN14PConnectionEnd15HandleTimerTickEv, 23
    Dispatch _ZN14PConnectionEnd13HandleRequestEPhmR10TUMsgTokenm, 24
    Dispatch _ZN14PConnectionEnd11HandleReplyEmm, 25
    Dispatch _ZN14PConnectionEnd16KillControlStartEv, 26
    Dispatch _ZN14PConnectionEnd21KillGetCommEventStartEv, 27
    Dispatch _ZN14PConnectionEnd12GetCommEventEv, 28
    Dispatch _ZN14PConnectionEnd18ProcessOptionStartEP7TOptionmm, 29
    Dispatch _ZN14PConnectionEnd26ProcessPutBytesOptionStartEP7TOptionmm, 30
    Dispatch _ZN14PConnectionEnd26ProcessGetBytesOptionStartEP7TOptionmm, 31
    Dispatch _ZN14PConnectionEnd18ResArbReleaseStartEPhS0_, 32
    Dispatch _ZN14PConnectionEnd23ResArbClaimNotificationEPhS0_, 33
    Dispatch _ZN14PConnectionEnd14ForwardOptionsEv, 34
