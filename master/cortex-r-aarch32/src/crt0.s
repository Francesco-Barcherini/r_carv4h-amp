.text

.globl  _start
.globl  _exit.crt0
.globl  watchdogTrigger
.syntax unified

.macro  switch_el, xreg, el2_label, el1_label, el0_label
    mrs     \xreg, CPSR
    and     \xreg, \xreg, #0x1f
    cmp     \xreg, 0x1a
    beq     \el2_label
    cmp     \xreg, 0x10
    beq     \el0_label
    b       \el1_label
.endm

# define _LO(x) (x&0xffff)
# define _HI(x) ((x>>16)&0xffff)

.macro WEAK_FUNCTION name
.weak \name
#ifdef __thumb__
.thumb_func
#endif
\name :
.endm

_start:
    ldr r0, .ISR_VECTOR_BASE
    switch_el r7, 2f, 1f, 0f
2:
    /* EL2 */
    /* init HSCTLR and HVBAR */
    mcr    p15,0x4,r0,c12,c0,0x0
    ldr    r0, .HSCTLR_VALUE
    mrc    p15,0x4,r1,c1,c0,0x0
    mov    r2, #0x1
    and    r1, r1, r2
    orr    r0, r0, r1
    mcr    p15,0x4,r0,c1,c0,0x0
    b el2_el1
1:
    /* EL1 */
    /* init SCTLR and VBAR */
    mcr    p15,0x0,r0,c12,c0,0x0
    ldr    r0, .SCTLR_VALUE
    mrc    p15,0x0,r1,c1,c0,0x0
    mov    r2, #0x1
    and    r1, r1, r2
    orr    r0, r0, r1
    mcr    p15,0x0,r0,c1,c0,0x0
    /* switch to SVC */
    cps    #0x13
el2_el1:
#if !defined(__SOFTFP__)
    bl     fpu_init_el1
#endif
    /* check MPIDR - jump to _start_secondary*/
    mrc    p15,0x0,r0,c0,c0,0x5
#if defined(BOOTMPIDR)
    lsl    r0, r0, #0x10
    lsr    r0, r0, #0x10
    movw   r1, #_LO(BOOTMPIDR)
    cmp    r0, r1
#else
    mov    r1, #1
    ands   r0, r1, r0
#endif
    bne    _start_secondary
0:  
    /* EL0 */
    /* set stack and frame pointer */
    ldr    r1, .SP
    mov    r0, #7
    bic    r1, r0 
    mov    r0, #0
    mov    sp, r1
    mov    fp, r0
    mov    lr, r0

    /* early initialize Watchdog */
    ldr    r3, .WDG_INIT_EARLY
    cmp    r3, #0
    beq    wdg_init_early_done
    bl     crt0_veneer
wdg_init_early_done:

#if defined(RAM_ECC_QUAD)||defined(RAM_ECC_LONG)||defined(RAM_ECC_SHORT)
    movw   r2, #_LO(RAMSTART)
    movt   r2, #_HI(RAMSTART)
    movw   r1, #_LO(RAMSIZE)
    movt   r1, #_HI(RAMSIZE)
    ldr    r4, .DATA_S
    ldr    r5, .DATA_LADDR
    cmp    r4, r5
    b      ecc_check
ecc_clear:
#if defined(RAM_ECC_QUAD)
    strd   r0, [r2], #0x8
    subs   r1, r1, #0x8
#elif defined(RAM_ECC_LONG)
    str    r0, [r2], #0x4
    subs   r1, r1, #0x4
#elif defined(RAM_ECC_SHORT)
    strh   r0, [r2], #0x2
    subs   r1, r1, #0x2
#endif
ecc_check:
    bne    ecc_clear
#endif

    /* copy data from FLASH to SRAM */
    ldr    r4, .DATA_S
    ldr    r5, .DATA_LADDR
    ldr    r6, .DATA_SIZE
    cmp    r4, r5
    beq    data_done
data_copy:
    ldrb   r0, [r5], #0x1
    strb   r0, [r4], #0x1
    subs   r6, r6, #1
    bne    data_copy
data_done:

    /* clear .bss-section */
    ldr    r4, .BSS_S
    ldr    r5, .BSS_E
    mov    r0, #0
    b      bss_check
bss_clear:
    strb   r0, [r4], #0x1
bss_check:
    cmp    r5, r4
    bne    bss_clear
bss_done:

    /* initialize Watchdog */
    ldr    r3, .WDG_INIT
    cmp    r3, #0
    beq    wdg_init_done
    bl     crt0_veneer
wdg_init_done:
    /* register the WatchdogHook */
    ldr    r3, .WDG_TRIGGER
    ldr    r4, .WDG_HOOK
    str    r3, [r4]

    /* initialization of e.g. static objects */
    ldr    r4, .INIT_S
    ldr    r5, .INIT_E
    b      init_check
init:
    ldr    r3,[r4], #0x4
    bl     crt0_veneer
init_check:
    cmp    r5, r4
    bne    init
init_done:

gomain:
    ldr    r3, .MAIN
    bl     crt0_veneer

_exit.crt0:
    /* we should never get here */
    b     _exit.crt0

    /* intended fallthrough for all weak default handlers */
WEAK_FUNCTION Undef_Handler
WEAK_FUNCTION SWI_Handler
WEAK_FUNCTION PAbort_Handler
WEAK_FUNCTION DAbort_Handler
WEAK_FUNCTION HypCall_Handler
WEAK_FUNCTION IRQ_Handler
WEAK_FUNCTION FIQ_Handler
_hang:
    /* default fault handler */
    b     _hang

crt0_veneer:
    /* make a THUMB/ARM generic veneer call to r3 */
    /* lr is set by preceding bl instruction      */
    mov    r0, #0
    mov    r1, #0
    bx     r3
    
.org 0x180
_start_secondary:
    /* set stack and frame pointer */
    ldr    r1, .SP_MP
    mov    r0, #7
    bic    r1, r0
    mov    r0, #0
    mov    sp, r1
    mov    fp, r0
    mov    r0, #1
    ldr    r3, .BACKGROUND
    blx    r3
    b      _exit.crt0
    
#if !defined(__SOFTFP__)
fpu_init_el1:
    ldr    r1, .CPACR_VALUE
    mrc    p15,0x0,r0,c1,c0,0x2
    orr    r0, r0, r1
    mcr    p15,0x0,r0,c1,c0,0x2
    ldr    r0, .FPEXC_VALUE
    vmsr   FPEXC, r0
    bx     r14
#endif

.align  2
.SP:
    .word __stack_end
.SP_MP:
    .word __stack_end_mp
.BSS_S:
    .word __bss_start
.BSS_E:
    .word __bss_end
.INIT_S:
    .word __init_array_start
.INIT_E:
    .word __init_array_end
.DATA_S:
    .word __data_vaddr
.DATA_LADDR:
    .word __data_laddr
.DATA_SIZE:
    .word __data_size
#ifndef WATCHDOG
.WDG_INIT:
    .word 0x0
.WDG_INIT_EARLY:
    .word 0x0
.WDG_TRIGGER:
    .word 0x0
#else
.WDG_INIT:
    .word WatchdogInit
.WDG_INIT_EARLY:
    .word WatchdogInitEarly
.WDG_TRIGGER:
    .word WatchdogTrigger
#endif
.WDG_HOOK:
    .word watchdogTrigger
.MAIN:
    .word main
.BACKGROUND:
    .word background
#ifdef __ARM_BIG_ENDIAN
#ifdef __thumb__
.SCTLR_VALUE:
    .word 0x72C50838
.HSCTLR_VALUE:
    .word 0x72C50838
#else
.SCTLR_VALUE:
    .word 0x32C50838
.HSCTLR_VALUE:
    .word 0x32C50838
#endif
#else
#ifdef __thumb__
.SCTLR_VALUE:
    .word 0x70C50838
.HSCTLR_VALUE:
    .word 0x70C50838
#else
.SCTLR_VALUE:
    .word 0x30C50838 
.HSCTLR_VALUE:
    .word 0x30C50838
#endif
#endif
#if defined(__ARM_FP)
.CPACR_VALUE:
    .word 0x00f00000
.FPEXC_VALUE:
    .word 0x40000000
#endif
.ISR_VECTOR_BASE:
    .word isr_vector_base

.section .isr_vector,"ax"
isr_vector_base:
    b _start
    b Undef_Handler
    b SWI_Handler
    b PAbort_Handler
    b DAbort_Handler
    b HypCall_Handler
    b IRQ_Handler
    b FIQ_Handler

.section .data
watchdogTrigger:
    .word 0x0

.section .text
.weak background
background:
    nop
1:
    wfe
    b 1b

