    .arch armv3
    .balign 4

    .section ".text.EinsteinLog", "ax"
    .global EinsteinLog
    .global strlen

EinsteinLog:
    stmdb   sp!, {r0, lr}
    bl      strlen
    ldmia   sp!, {r1, lr}
    mov     r0, #0x100
    add     r0, #0x01A
    mcr     p10, 0, r0, c0, c0
    mov     pc, lr

    .section ".text.EinsteinBreakInMonitor", "ax"
    .global EinsteinBreakInMonitor

EinsteinBreakInMonitor:
    mov     r0, #0x100
    add     r0, #0x016
    mcr     p10, 0, r0, c0, c0
    mov     pc, lr
