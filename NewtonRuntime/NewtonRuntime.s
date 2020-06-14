    .fpu fpa10
    .arch armv3
    .arm
    . = 0
    .balign 4

    .section ".text.snprintf"
    .global snprintf
    .global snprintf_
snprintf:
    b   snprintf_

// Implementation of some aebi callbacks
// Untested and incomplete. Only provided for some printf cases we probably
// don't actually need.

// double __aeabi_dadd(double, double)	double-precision addition
    .section ".text.__aeabi_dadd", "ax"
    .global __aeabi_dadd
__aeabi_dadd:
    stmdb r13!, {r0-r3}
    lfmea f0, 2, [r13]!
    adfd f0, f0, f1
    stfd f0, [r13]!
    ldmea r13!, {r0-r1}
    mov pc, lr

// double __aeabi_ddiv(double n, double d)	double-precision division, n / d
    .section ".text.__aeabi_ddiv", "ax"
    .global __aeabi_ddiv
__aeabi_ddiv:
    stmdb r13!, {r0-r3}
    lfmea f0, 2, [r13]!
    dvfd f0, f0, f1
    stfd f0, [r13]!
    ldmea r13!, {r0-r1}
    mov pc, lr

// double __aeabi_dmul(double, double)	double-precision multiplication
    .section ".text.__aeabi_dmul", "ax"
    .global __aeabi_dmul
__aeabi_dmul:
    stmdb r13!, {r0-r3}
    lfmea f0, 2, [r13]!
    mufd f0, f0, f1
    stfd f0, [r13]!
    ldmea r13!, {r0-r1}
    mov pc, lr

// double __aeabi_drsub(double x, double y)	double-precision reverse subtraction, y - x
    .section ".text.__aeabi_drsub", "ax"
    .global __aeabi_drsub
__aeabi_drsub:
    stmdb r13!, {r0-r3}
    lfmea f0, 2, [r13]!
    rsfd f0, f0, f1
    stfd f0, [r13]!
    ldmea r13!, {r0-r1}
    mov pc, lr

// double __aeabi_dsub(double x, double y)	double-precision subtraction, x - y
    .section ".text.__aeabi_dsub", "ax"
    .global __aeabi_dsub
__aeabi_dsub:
    stmdb r13!, {r0-r3}
    lfmea f0, 2, [r13]!
    sufd f0, f0, f1
    stfd f0, [r13]!
    ldmea r13!, {r0-r1}
    mov pc, lr

// void __aeabi_cdcmpeq(double, double)	non-excepting equality comparison [1], result in PSR ZC flags
    .section ".text.__aeabi_cdcmpeq", "ax"
    .global __aeabi_cdcmpeq
__aeabi_cdcmpeq:
    stmdb r13!, {r0-r3}
    lfmea f0, 2, [r13]!
    cmf f0, f1
    mov pc, lr

// void __aeabi_cdcmple(double, double)	3-way (<, =, ?>) compare [1], result in PSR ZC flags
    .section ".text.__aeabi_cdcmple", "ax"
    .global __aeabi_cdcmple
__aeabi_cdcmple:
    cmfe f0, f1
    mov pc, lr

// void __aeabi_cdrcmple(double, double)	reversed 3-way (<, =, ?>) compare [1], result in PSR ZC flags
    .section ".text.__aeabi_cdrcmple", "ax"
    .global __aeabi_cdrcmple
__aeabi_cdrcmple:
    stmdb r13!, {r0-r3}
    lfmea f0, 2, [r13]!
    cnfe f0, f1
    mov pc, lr

// int __aeabi_dcmpeq(double, double)	result (1, 0) denotes (=, ?<>) [2], use for C == and !=
    .section ".text.__aeabi_dcmpeq", "ax"
    .global __aeabi_dcmpeq
__aeabi_dcmpeq:
    stmdb r13!, {r0-r3}
    lfmea f0, 2, [r13]!
    cmf f0, f1
    moveq r0, #1
    mov pc, lr

// int __aeabi_dcmplt(double, double)	result (1, 0) denotes (<, ?>=) [2], use for C <
    .section ".text.__aeabi_dcmplt", "ax"
    .global __aeabi_dcmplt
__aeabi_dcmplt:
    stmdb r13!, {r0-r3}
    lfmea f0, 2, [r13]!
    cmfe f0, f1
    movlt r0, #1
    mov pc, lr

// int __aeabi_dcmple(double, double)	result (1, 0) denotes (<=, ?>) [2], use for C <=
    .section ".text.__aeabi_dcmple", "ax"
    .global __aeabi_dcmple
__aeabi_dcmple:
    stmdb r13!, {r0-r3}
    lfmea f0, 2, [r13]!
    cmfe f0, f1
    movle r0, #1
    mov pc, lr

// int __aeabi_dcmpge(double, double)	result (1, 0) denotes (>=, ?<) [2], use for C >=
    .section ".text.__aeabi_dcmpge", "ax"
    .global __aeabi_dcmpge
__aeabi_dcmpge:
    stmdb r13!, {r0-r3}
    lfmea f0, 2, [r13]!
    cmfe f0, f1
    movge r0, #1
    mov pc, lr

// int __aeabi_dcmpgt(double, double)	result (1, 0) denotes (>, ?<=) [2], use for C >
    .section ".text.__aeabi_dcmpgt", "ax"
    .global __aeabi_dcmpgt
__aeabi_dcmpgt:
    stmdb r13!, {r0-r3}
    lfmea f0, 2, [r13]!
    cmfe f0, f1
    movgt r0, #1
    mov pc, lr

// int __aeabi_dcmpun(double, double)	result (1, 0) denotes (?, <=>) [2], use for C99 isunordered()
    .section ".text.__aeabi_dcmpun", "ax"
    .global __aeabi_dcmpun
__aeabi_dcmpun:
    stmdb r13!, {r0-r3}
    lfmea f0, 2, [r13]!
    cmf f0, f1
    movvs r0, #1
    mov pc, lr

// double __aeabi_i2d(int)	integer to double conversion    // FIX SIGN ISSUE
    .section ".text.__aeabi_i2d", "ax"
    .global __aeabi_i2d
__aeabi_i2d:
    fltd f0, r0
    stfd f0, [r13]!
    ldmea r13!, {r0-r1}
    mov pc, lr

// double __aeabi_ui2d(unsigned)	unsigned to double conversion
    .section ".text.__aeabi_ui2d", "ax"
    .global __aeabi_ui2d
__aeabi_ui2d:
    fltd f0, r0
    stfd f0, [r13]!
    ldmea r13!, {r0-r1}
    mov pc, lr

    .section ".text.__aeabi_uldivmod", "ax"
    .global __aeabi_uldivmod
__aeabi_uldivmod:
    mov pc, lr

    .section ".text.__aeabi_d2uiz", "ax"
    .global __aeabi_d2uiz
__aeabi_d2uiz:
    mov pc, lr

    .section ".text.__aeabi_d2iz", "ax"
    .global __aeabi_d2iz
__aeabi_d2iz:
    mov pc, lr
