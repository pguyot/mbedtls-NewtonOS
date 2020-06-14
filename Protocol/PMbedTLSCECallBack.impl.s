    .arch armv3
    .balign 4

    .global _ZN18PMbedTLSCECallBack9ClassInfoEv

local___ZN18PMbedTLSCECallBack9ClassInfoEv:
@ (reserved for now)
    .word   0
    .word   __implname__ - .
    .word   __protname__ - .
    .word   __cap__ - .
    .word   __btbl__ - .
    .word   __monent__ - .
@ branch to sizeof glue
    b       _ZN18PMbedTLSCECallBack6SizeofEv
@ zero, or branch to Alloc method
    .word   0
@ zero, or branch to Free method
    .word   0
@ return, or branch to New(void)
    mov     pc, lr
@ return, or branch to Delete
    b   _ZN18PMbedTLSCECallBack6DeleteEv
@ version number
    .word   0x00000000
@ flags
    .word   0x00000000
@ (reserved)
    .word   0x00000000
@ (reserved)
    mov     r0, #0
    mov     pc, lr

_ZN18PMbedTLSCECallBack9ClassInfoEv:
    adr     r0, local___ZN18PMbedTLSCECallBack9ClassInfoEv
    mov     pc, lr

__implname__:
    .string "PMbedTLSCECallBack"
__protname__:
    .string "PCECallBack"
    .balign 4
__cap__:
    .word   0
__btbl__:
    .word   0
    b   _ZN18PMbedTLSCECallBack9ClassInfoEv
    mov     pc, lr  /* new */
    b   _ZN18PMbedTLSCECallBack6DeleteEv

    b   _ZN18PMbedTLSCECallBack4InitEP14TConnectionEnd
    b   _ZN18PMbedTLSCECallBack12OpenCompleteEl
    b   _ZN18PMbedTLSCECallBack13CloseCompleteEl
    b   _ZN18PMbedTLSCECallBack12BindCompleteEl
    b   _ZN18PMbedTLSCECallBack14UnbindCompleteEl
    b   _ZN18PMbedTLSCECallBack15ConnectCompleteEl
    b   _ZN18PMbedTLSCECallBack14ListenCompleteEl
    b   _ZN18PMbedTLSCECallBack14AcceptCompleteEl
    b   _ZN18PMbedTLSCECallBack17TerminateCompleteEv
    b   _ZN18PMbedTLSCECallBack11PutCompleteElm
    b   _ZN18PMbedTLSCECallBack15KillPutCompleteEl
    b   _ZN18PMbedTLSCECallBack11GetCompleteElhm
    b   _ZN18PMbedTLSCECallBack15KillGetCompleteEl
    b   _ZN18PMbedTLSCECallBack13HandleRequestEPhmR10TUMsgTokenm
    b   _ZN18PMbedTLSCECallBack11HandleReplyEmm
    b   _ZN18PMbedTLSCECallBack19KillControlCompleteEl
    b   _ZN18PMbedTLSCECallBack24KillGetCommEventCompleteEl
    b   _ZN18PMbedTLSCECallBack12GetCommEventEv
    b   _ZN18PMbedTLSCECallBack13PostCommEventER22TCommToolGetEventReplyl
    b   _ZN18PMbedTLSCECallBack21ProcessOptionCompleteEm
    b   _ZN18PMbedTLSCECallBack29ProcessPutBytesOptionCompleteEm
    b   _ZN18PMbedTLSCECallBack29ProcessGetBytesOptionCompleteEm
    b   _ZN18PMbedTLSCECallBack21ResArbReleaseCompleteEl
    b   _ZN18PMbedTLSCECallBack12FlushChannelEml
    b   _ZN18PMbedTLSCECallBack10StartAbortEl
    b   _ZN18PMbedTLSCECallBack11GetAbortErrEv
    b   _ZN18PMbedTLSCECallBack19GetTerminationEventEv
    b   _ZN18PMbedTLSCECallBack19SetTerminationEventEm
    b   _ZN18PMbedTLSCECallBack15InitAsyncRPCMsgER14TUAsyncMessagem
    b   _ZN18PMbedTLSCECallBack19GetToolConnectStateEv
    b   _ZN18PMbedTLSCECallBack19SetToolConnectStateEm
    b   _ZN18PMbedTLSCECallBack14GetConnectInfoEv
    b   _ZN18PMbedTLSCECallBack14SetConnectInfoER17TCMOCTConnectInfo
    b   _ZN18PMbedTLSCECallBack19GetCMOTransportInfoEv
    b   _ZN18PMbedTLSCECallBack19SetCMOTransportInfoER17TCMOTransportInfo
    b   _ZN18PMbedTLSCECallBack24GetReceiveMessageBufSizeEv
    b   _ZN18PMbedTLSCECallBack20GetConnectionEndPortEv
    b   _ZN18PMbedTLSCECallBack21GetControlOptionsInfoEv
    b   _ZN18PMbedTLSCECallBack22GetGetBytesOptionsInfoEv
    b   _ZN18PMbedTLSCECallBack22GetPutBytesOptionsInfoEv
    b   _ZN18PMbedTLSCECallBack13GetCurPutDataEv
    b   _ZN18PMbedTLSCECallBack18GetCurPutFrameDataEv
    b   _ZN18PMbedTLSCECallBack19GetCurPutEndOfFrameEv
    b   _ZN18PMbedTLSCECallBack13GetCurGetDataEv
    b   _ZN18PMbedTLSCECallBack18GetCurGetFrameDataEv
    b   _ZN18PMbedTLSCECallBack20GetCurGetNonBlockingEv
    b   _ZN18PMbedTLSCECallBack18GetCurGetThresholdEv
    b   _ZN18PMbedTLSCECallBack15GetPassiveClaimEv
    b   _ZN18PMbedTLSCECallBack15SetPassiveClaimEh
    b   _ZN18PMbedTLSCECallBack15GetPassiveStateEv
    b   _ZN18PMbedTLSCECallBack15SetPassiveStateEh
    b   _ZN18PMbedTLSCECallBack22GetWaitingForResNotifyEv
    b   _ZN18PMbedTLSCECallBack22SetWaitingForResNotifyEh
    b   _ZN18PMbedTLSCECallBack19GetCurRequestOpCodeEv

__monent__:

_ZN18PMbedTLSCECallBack4InitEP14TConnectionEnd:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd6InitCBEP14TConnectionEnd

_ZN18PMbedTLSCECallBack12OpenCompleteEl:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd14OpenCompleteCBEl

_ZN18PMbedTLSCECallBack13CloseCompleteEl:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd15CloseCompleteCBEl

_ZN18PMbedTLSCECallBack12BindCompleteEl:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd14BindCompleteCBEl

_ZN18PMbedTLSCECallBack14UnbindCompleteEl:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd16UnbindCompleteCBEl

_ZN18PMbedTLSCECallBack15ConnectCompleteEl:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd17ConnectCompleteCBEl

_ZN18PMbedTLSCECallBack14ListenCompleteEl:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd16ListenCompleteCBEl

_ZN18PMbedTLSCECallBack14AcceptCompleteEl:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd16AcceptCompleteCBEl

_ZN18PMbedTLSCECallBack17TerminateCompleteEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd19TerminateCompleteCBEv

_ZN18PMbedTLSCECallBack11PutCompleteElm:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd13PutCompleteCBElm

_ZN18PMbedTLSCECallBack15KillPutCompleteEl:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd17KillPutCompleteCBEl

_ZN18PMbedTLSCECallBack11GetCompleteElhm:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd13GetCompleteCBElhm

_ZN18PMbedTLSCECallBack15KillGetCompleteEl:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd17KillGetCompleteCBEl

_ZN18PMbedTLSCECallBack13HandleRequestEPhmR10TUMsgTokenm:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd15HandleRequestCBEPhmR10TUMsgTokenm

_ZN18PMbedTLSCECallBack11HandleReplyEmm:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd13HandleReplyCBEmm

_ZN18PMbedTLSCECallBack19KillControlCompleteEl:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd21KillControlCompleteCBEl

_ZN18PMbedTLSCECallBack24KillGetCommEventCompleteEl:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd26KillGetCommEventCompleteCBEl

_ZN18PMbedTLSCECallBack12GetCommEventEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd14GetCommEventCBEv

_ZN18PMbedTLSCECallBack13PostCommEventER22TCommToolGetEventReplyl:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd15PostCommEventCBER22TCommToolGetEventReplyl

_ZN18PMbedTLSCECallBack21ProcessOptionCompleteEm:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd23ProcessOptionCompleteCBEm

_ZN18PMbedTLSCECallBack29ProcessPutBytesOptionCompleteEm:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd31ProcessPutBytesOptionCompleteCBEm

_ZN18PMbedTLSCECallBack29ProcessGetBytesOptionCompleteEm:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd31ProcessGetBytesOptionCompleteCBEm

_ZN18PMbedTLSCECallBack21ResArbReleaseCompleteEl:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd23ResArbReleaseCompleteCBEl

_ZN18PMbedTLSCECallBack12FlushChannelEml:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd14FlushChannelCBEml

_ZN18PMbedTLSCECallBack10StartAbortEl:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd12StartAbortCBEl

_ZN18PMbedTLSCECallBack11GetAbortErrEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd13GetAbortErrCBEv

_ZN18PMbedTLSCECallBack19GetTerminationEventEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd21GetTerminationEventCBEv

_ZN18PMbedTLSCECallBack19SetTerminationEventEm:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd21SetTerminationEventCBEm

_ZN18PMbedTLSCECallBack15InitAsyncRPCMsgER14TUAsyncMessagem:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd17InitAsyncRPCMsgCBER14TUAsyncMessagem

_ZN18PMbedTLSCECallBack19GetToolConnectStateEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd21GetToolConnectStateCBEv

_ZN18PMbedTLSCECallBack19SetToolConnectStateEm:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd21SetToolConnectStateCBEm

_ZN18PMbedTLSCECallBack14GetConnectInfoEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd16GetConnectInfoCBEv

_ZN18PMbedTLSCECallBack14SetConnectInfoER17TCMOCTConnectInfo:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd16SetConnectInfoCBER17TCMOCTConnectInfo

_ZN18PMbedTLSCECallBack19GetCMOTransportInfoEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd21GetCMOTransportInfoCBEv

_ZN18PMbedTLSCECallBack19SetCMOTransportInfoER17TCMOTransportInfo:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd21SetCMOTransportInfoCBER17TCMOTransportInfo

_ZN18PMbedTLSCECallBack24GetReceiveMessageBufSizeEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd26GetReceiveMessageBufSizeCBEv

_ZN18PMbedTLSCECallBack20GetConnectionEndPortEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd22GetConnectionEndPortCBEv

_ZN18PMbedTLSCECallBack21GetControlOptionsInfoEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd23GetControlOptionsInfoCBEv

_ZN18PMbedTLSCECallBack22GetGetBytesOptionsInfoEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd24GetGetBytesOptionsInfoCBEv

_ZN18PMbedTLSCECallBack22GetPutBytesOptionsInfoEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd24GetPutBytesOptionsInfoCBEv

_ZN18PMbedTLSCECallBack13GetCurPutDataEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd15GetCurPutDataCBEv

_ZN18PMbedTLSCECallBack18GetCurPutFrameDataEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd20GetCurPutFrameDataCBEv

_ZN18PMbedTLSCECallBack19GetCurPutEndOfFrameEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd21GetCurPutEndOfFrameCBEv

_ZN18PMbedTLSCECallBack13GetCurGetDataEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd15GetCurGetDataCBEv

_ZN18PMbedTLSCECallBack18GetCurGetFrameDataEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd20GetCurGetFrameDataCBEv

_ZN18PMbedTLSCECallBack20GetCurGetNonBlockingEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd22GetCurGetNonBlockingCBEv

_ZN18PMbedTLSCECallBack18GetCurGetThresholdEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd20GetCurGetThresholdCBEv

_ZN18PMbedTLSCECallBack15GetPassiveClaimEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd17GetPassiveClaimCBEv

_ZN18PMbedTLSCECallBack15SetPassiveClaimEh:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd17SetPassiveClaimCBEh

_ZN18PMbedTLSCECallBack15GetPassiveStateEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd17GetPassiveStateCBEv

_ZN18PMbedTLSCECallBack15SetPassiveStateEh:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd17SetPassiveStateCBEh

_ZN18PMbedTLSCECallBack22GetWaitingForResNotifyEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd24GetWaitingForResNotifyCBEv

_ZN18PMbedTLSCECallBack22SetWaitingForResNotifyEh:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd24SetWaitingForResNotifyCBEh

_ZN18PMbedTLSCECallBack19GetCurRequestOpCodeEv:
    ldr    r0, [r0, #16]
    b    _ZN21PMbedTLSConnectionEnd21GetCurRequestOpCodeCBEv


    .end
