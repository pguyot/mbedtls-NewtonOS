    .arch armv3
    .balign 4

    .global _ZN19TAEventHandlerProxy15kVirtualTable21E

_ZN19TAEventHandlerProxy15kVirtualTable21E:
    b   __dt__14TAEventHandlerFv
    b   _ZN19TAEventHandlerProxy13__AETestEventEPNS_21SAEHandlerWPtrToProxyEP7TAEvent
    b   _ZN19TAEventHandlerProxy15__AEHandlerProcEPNS_21SAEHandlerWPtrToProxyEP10TUMsgTokenPmP7TAEvent
    b   _ZN19TAEventHandlerProxy18__AECompletionProcEPNS_21SAEHandlerWPtrToProxyEP10TUMsgTokenPmP7TAEvent
    b   _ZN19TAEventHandlerProxy10__IdleProcEPNS_21SAEHandlerWPtrToProxyEP10TUMsgTokenPmP7TAEvent

