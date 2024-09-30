
/*
 * Branch according to exception level
 * [taken from U-Boot]
 */
.macro  switch_el, xreg, el3_label, el2_label, el1_label
    mrs     \xreg, CurrentEL
    cmp     \xreg, 0xc
    b.eq    \el3_label
    cmp     \xreg, 0x8
    b.eq    \el2_label
    cmp     \xreg, 0x4
    b.eq    \el1_label
.endm

/*
 * Define two symbols - one weak and one strong with _vector suffix
 */
.macro WEAK_VECTOR name
.weak \name
\name :
\name\()_vector:
.endm

.text

.globl  _start
.globl  _exit.crt0
.globl  watchdogTrigger
.globl  isr_vector_base


_start:
    /* set stack and frame pointer */
    ldr     x1,  .SP
    mov     x0,  #0xf
    bic     x1,  x1,  x0
    msr     spsel, #1
    mov     sp,  x1
    mov     x29, #0x0
    
    ldr     x0, .ISR_VECTOR_BASE
    switch_el x1, 3f, 2f, 1f
3:  /* EL3 */
    msr     vbar_el3, x0
    msr     DAIFSet, #0x3
#if !defined(__SOFTFP__)
    msr     cptr_el3, xzr
#endif
    b       0f
    
2:  /* EL2 */
    msr     vbar_el2, x0
    msr     DAIFSet, #0x3
#if !defined(__SOFTFP__)
    /* enable FPU */
    mov     x0,  #0x33ff
    msr     cptr_el2, x0
#endif
    b       0f
    
1:  /* EL1 */
    msr     vbar_el1, x0
    msr     DAIFSet, #0x3
#if !defined(__SOFTFP__)
    /* enable FPU */
    mov     x0,  #3 << 20
    msr     cpacr_el1, x0
#endif

0: 
    /* Check MultiprocessorID */
    mrs	    x0,  mpidr_el1
#if defined(BOOTMPIDR)
    and     x0,  x0, #0xffff
    cmp     x0,  #BOOTMPIDR
#else
    ands    x0,  x0, #0xf
#endif
    b.ne    _start_secondary

    /* copy data from FLASH to SRAM */
    ldr    x4, .DATA_S
    ldr    x5, .DATA_LADDR
    ldr    x6, .DATA_SIZE
    cmp    x4, x5
    b.eq   data_done
data_copy:
    ldrb   w0, [x5], #0x1
    strb   w0, [x4], #0x1
    subs   x6, x6, #0x1
    b.ne   data_copy
data_done:

bss_init:
    /* clear .bss-section */
    ldr     x4,  .BSS_S
    ldr     x5,  .BSS_E
    b       bss_check
bss_clear:
    strb    wzr, [x4], #0x1
bss_check:
    cmp     x5,  x4
    b.ne    bss_clear
bss_done:

    /* initialize Watchdog */
    ldr     x3,  .WDG_INIT
    cmp     x3,  #0
    b.eq    wdg_init_done
    bl      crt0_veneer
wdg_init_done:
    /* register the WatchdogHook */
    ldr     x3,  .WDG_TRIGGER
    ldr     x4,  .WDG_HOOK
    str     x3,  [x4]

    /* initialization of e.g. static objects */
    ldr     x19, .INIT_S
    ldr     x20, .INIT_LADDR
    ldr     x21, .INIT_SIZE
    add     x22, x21, #0x8
    cmp     x19, x20
    b.eq    init_check
init_copy:
    subs    x21, x21, 0x1
    ldrb    w0,  [x20, x21]
    strb    w0,  [x19, x21]
    b.ne    init_copy
    b       init_check
init:
    ldr     x3,  [x19], #0x8
    bl      crt0_veneer
init_check:
    subs    x22, x22, #0x8
    b.ne    init
init_done:

gomain:
    ldr     x3,  .MAIN
    bl      crt0_veneer
    nop

_exit.crt0:
    b       _exit.crt0
    
_hang:
    b       _hang

crt0_veneer:
    /* make a generic veneer call to r3 */
    /* lr is set by preceding bl instruction      */
    mov     x0,  #0
    mov     x1,  #0
    br      x3

_start_secondary:
    ldr     x1,  .STACK_SIZE
    mul     x0,  x0, x1
    ldr     x1,  .SP_MP
    add     x0,  x0, x1
    mov     x1,  #0xf
    bic     x0,  x0,  x1
    mov     sp,  x0
    mov     x29, #0
    ldr     x3,  .BACKGROUND
    bl      crt0_veneer
    nop
    b       _exit.crt0
.align  3
.SP:
    .quad   __stack_end
.SP_MP:
    .quad   __stack_start_mp
.STACK_SIZE:
    .quad   __stack_size
.BSS_S:
    .quad   __bss_start
.BSS_E:
    .quad   __bss_end
.INIT_S:
    .quad   __init_vaddr
.INIT_LADDR:
    .quad   __init_laddr
.INIT_SIZE:
    .quad   __init_size
.DATA_S:
    .quad   __data_vaddr
.DATA_LADDR:
    .quad   __data_laddr
.DATA_SIZE  :
    .quad   __data_size
#ifndef WATCHDOG
.WDG_INIT:
    .quad   0x0
.WDG_TRIGGER:
    .quad   0x0
#else
.WDG_INIT:
    .quad   WatchdogInit
.WDG_TRIGGER:
    .quad   WatchdogTrigger
#endif
.WDG_HOOK:
    .quad   watchdogTrigger
.MAIN:
    .quad   main
.BACKGROUND:
    .quad   background
.ISR_VECTOR_BASE:
    .quad   isr_vector_base

/*
 * generate a exception vector table
 */
.section .isr_vector,"ax"
isr_vector_base:
    .org 0x000
WEAK_VECTOR syncExceptionSP0
    b       syncExceptionSP0    /* Current EL Synchronous Thread SP0 */

    .org 0x080
WEAK_VECTOR irqSP0
    b       irqSP0              /* Current EL IRQ Thread SP0 */

    .org 0x100
WEAK_VECTOR fiqSP0
    b       fiqSP0              /* Current EL FIQ Thread SP0 */

    .org 0x180
WEAK_VECTOR errorSP0
    b       errorSP0            /* Current EL Error Thread SP0 */

    .org 0x200
WEAK_VECTOR syncExceptionSPx
    b       syncExceptionSPx    /* Current EL Synchronous Handler SPx */

    .org 0x280
WEAK_VECTOR irqSPx
    b       irqSPx              /* Current EL IRQ Handler SPx */

    .org 0x300
WEAK_VECTOR fiqSPx
    b       fiqSPx              /* Current EL FIQ Handler SPx */

    .org 0x380
WEAK_VECTOR errorSPx
    b       errorSPx            /* Current EL Error Handler SPx */

    .org 0x400
WEAK_VECTOR syncExceptionA64
    b       syncExceptionA64    /* Lower EL Synchronous Handler AARCH64 */

    .org 0x480
WEAK_VECTOR irqA64
    b       irqA64              /* Lower EL IRQ Handler AARCH64 */

    .org 0x500
WEAK_VECTOR fiqA64
    b       fiqA64              /* Lower EL FIQ Handler AARCH64 */

    .org 0x580
WEAK_VECTOR errorA64
    b       errorA64            /* Lower EL Error Handler AARCH64 */
    
    .org 0x600
WEAK_VECTOR syncExceptionA32
    b       syncExceptionA32    /* Lower EL Synchronous Handler AARCH32 */

    .org 0x680
WEAK_VECTOR irqA32
    b       irqA32              /* Lower EL IRQ Handler AARCH32 */

    .org 0x700
WEAK_VECTOR fiqA32
    b       fiqA32              /* Lower EL FIQ Handler AARCH32 */

    .org 0x780
WEAK_VECTOR errorA32
    b       errorA32            /* Lower EL Error Handler AARCH32 */


.section .data
.align  3
watchdogTrigger:
    .quad 0x0

.section .text
.weak background
background:
    nop
1:
    wfe
    b 1b
