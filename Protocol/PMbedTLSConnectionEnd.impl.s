    .arch armv3
    .balign 4

    .global _ZN21PMbedTLSConnectionEnd9ClassInfoEv

local___ZN21PMbedTLSConnectionEnd9ClassInfoEv:
@ (reserved for now)
    .word   0
    .word   __implname__ - .
    .word   __protname__ - .
    .word   __cap__ - .
    .word   __btbl__ - .
    .word   __monent__ - .
@ branch to sizeof glue
    b       _ZN21PMbedTLSConnectionEnd6SizeofEv
@ zero, or branch to Alloc method
    .word   0
@ zero, or branch to Free method
    .word   0
@ return, or branch to New(void)
    b       _ZN21PMbedTLSConnectionEnd3NewEv
@ return, or branch to Delete
    b       _ZN21PMbedTLSConnectionEnd6DeleteEv
@ version number
    .word   0x00000000
@ flags
    .word   0x00000000
@ (reserved)
    .word   0x00000000
@ (reserved)
    b       return__nil__21PMbedTLSConnectionEnd

_ZN21PMbedTLSConnectionEnd9ClassInfoEv:
    adr     r0,local___ZN21PMbedTLSConnectionEnd9ClassInfoEv
    mov     pc,r14

return__nil__21PMbedTLSConnectionEnd:
    mov     r0,#0
    mov     pc,r14

__implname__:
    .string "PInetToolCE"
__protname__:
    .string "PConnectionEnd"
    .balign 4
__cap__:
    .word   0
__btbl__:
    .word   0
    b   _ZN21PMbedTLSConnectionEnd9ClassInfoEv
    b   _ZN21PMbedTLSConnectionEnd3NewEv
    b   _ZN21PMbedTLSConnectionEnd6DeleteEv

    b   _ZN21PMbedTLSConnectionEnd4InitEP8PMuxToolP11PCECallBackm
    b   _ZN21PMbedTLSConnectionEnd21HandleInternalRequestEPv
    b   _ZN21PMbedTLSConnectionEnd19HandleInternalEventEv
    b   _ZN21PMbedTLSConnectionEnd9OpenStartEv
    b   _ZN21PMbedTLSConnectionEnd10CloseStartEv
    b   _ZN21PMbedTLSConnectionEnd9BindStartEv
    b   _ZN21PMbedTLSConnectionEnd11UnbindStartEv
    b   _ZN21PMbedTLSConnectionEnd12ConnectStartEv
    b   _ZN21PMbedTLSConnectionEnd11ListenStartEv
    b   _ZN21PMbedTLSConnectionEnd11AcceptStartEv
    b   _ZN21PMbedTLSConnectionEnd12ReleaseStartEv
    b   _ZN21PMbedTLSConnectionEnd14TerminateStartEv
    b   _ZN21PMbedTLSConnectionEnd13PutBytesStartEP11CBufferList
    b   _ZN21PMbedTLSConnectionEnd19PutFramedBytesStartEP11CBufferListh
    b   _ZN21PMbedTLSConnectionEnd12KillPutStartEv
    b   _ZN21PMbedTLSConnectionEnd13GetBytesStartEP11CBufferList
    b   _ZN21PMbedTLSConnectionEnd19GetFramedBytesStartEP11CBufferList
    b   _ZN21PMbedTLSConnectionEnd22GetBytesImmediateStartEP11CBufferListl
    b   _ZN21PMbedTLSConnectionEnd12KillGetStartEv
    b   _ZN21PMbedTLSConnectionEnd15HandleTimerTickEv
    b   _ZN21PMbedTLSConnectionEnd13HandleRequestEPhmR10TUMsgTokenm
    b   _ZN21PMbedTLSConnectionEnd11HandleReplyEmm
    b   _ZN21PMbedTLSConnectionEnd16KillControlStartEv
    b   _ZN21PMbedTLSConnectionEnd21KillGetCommEventStartEv
    b   _ZN21PMbedTLSConnectionEnd12GetCommEventEv
    b   _ZN21PMbedTLSConnectionEnd18ProcessOptionStartEP7TOptionmm
    b   _ZN21PMbedTLSConnectionEnd26ProcessPutBytesOptionStartEP7TOptionmm
    b   _ZN21PMbedTLSConnectionEnd26ProcessGetBytesOptionStartEP7TOptionmm
    b   _ZN21PMbedTLSConnectionEnd18ResArbReleaseStartEPhS0_
    b   _ZN21PMbedTLSConnectionEnd23ResArbClaimNotificationEPhS0_
    b   _ZN21PMbedTLSConnectionEnd14ForwardOptionsEv

__monent__:
    .end
