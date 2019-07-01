/*
 * exception/excvec_el2.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include "lib/asm.h"
#include "driver/aarch64/exception.h"

        .global         excvec_el2
        .section        ".text.excvec.el2", "ax", %progbits
        .type           excvec_el2, %function
        .balign         2048
excvec_el2:
        /*
         * current EL with SP0
         */

        /* Synchronous */
        .org            0x0000
        dsb             sy
        sub             sp,  sp,  #(NR_EXC_REGS * 8)
        stp             x1,  x2,  [sp,  #(EXC_X1 * 8)]
        add             x1,  sp,  #(NR_EXC_REGS * 8)
        stp             x1,  x0,  [sp,  #(EXC_SP * 8)]
        mov             x0 , #0x0000
        b               save_rest_registers

        /* IRQ */
        .org            0x0080
        dsb             sy
        sub             sp,  sp,  #(NR_EXC_REGS * 8)
        stp             x1,  x2,  [sp,  #(EXC_X1 * 8)]
        add             x1,  sp,  #(NR_EXC_REGS * 8)
        stp             x1,  x0,  [sp,  #(EXC_SP * 8)]
        mov             x0 , #0x0080
        b               save_rest_registers

        /* FIQ */
        .org            0x0100
        dsb             sy
        sub             sp,  sp,  #(NR_EXC_REGS * 8)
        stp             x1,  x2,  [sp,  #(EXC_X1 * 8)]
        add             x1,  sp,  #(NR_EXC_REGS * 8)
        stp             x1,  x0,  [sp,  #(EXC_SP * 8)]
        mov             x0 , #0x0100
        b               save_rest_registers

        /* SError */
        .org            0x0180
        dsb             sy
        sub             sp,  sp,  #(NR_EXC_REGS * 8)
        stp             x1,  x2,  [sp,  #(EXC_X1 * 8)]
        add             x1,  sp,  #(NR_EXC_REGS * 8)
        stp             x1,  x0,  [sp,  #(EXC_SP * 8)]
        mov             x0 , #0x0180
        b               save_rest_registers

        /*
         * current EL with SPx
         */

        /* Synchronous */
        .org            0x0200
        dsb             sy
        mrs             x16, MPIDR_EL1
        and             x16, x16, #0xff
        mov             x17, #STACK_SIZE
        madd            x16, x16, x17, x17
        adr             x17, exc_stack_region
        ldr             x16, [x17, x16]

        sub             x16, x16, #(NR_EXC_REGS * 8)
        stp             x1,  x2,  [x16, #(EXC_X1 * 8)]
        mov             x1,  sp
        mov             sp,  x16
        stp             x1,  x0,  [sp,  #(EXC_SP * 8)]
        mov             x0,  #0x0200
        b               save_rest_registers

        /* IRQ */
        .org            0x0280
        dsb             sy
        sub             sp,  sp,  #(NR_EXC_REGS * 8)
        stp             x1,  x2,  [sp,  #(EXC_X1 * 8)]
        add             x1,  sp,  #(NR_EXC_REGS * 8)
        stp             x1,  x0,  [sp,  #(EXC_SP * 8)]
        mov             x0 , #0x0180
        b               save_rest_registers

        /* FIQ */
        .org            0x0300
        dsb             sy
        sub             sp,  sp,  #(NR_EXC_REGS * 8)
        stp             x1,  x2,  [sp,  #(EXC_X1 * 8)]
        add             x1,  sp,  #(NR_EXC_REGS * 8)
        stp             x1,  x0,  [sp,  #(EXC_SP * 8)]
        mov             x0 , #0x0180
        b               save_rest_registers

        /* SError */
        .org            0x0380
        dsb             sy
        sub             sp,  sp,  #(NR_EXC_REGS * 8)
        stp             x1,  x2,  [sp,  #(EXC_X1 * 8)]
        add             x1,  sp,  #(NR_EXC_REGS * 8)
        stp             x1,  x0,  [sp,  #(EXC_SP * 8)]
        mov             x0 , #0x0180
        b               save_rest_registers

        /*
         * lower EL using AArch64
         */

        /* Synchronous */
        .org            0x0400
        dsb             sy
        svc             #0

        /* IRQ */
        .org            0x0480
        dsb             sy
        svc             #0

        /* FIQ */
        .org            0x0500
        dsb             sy
        svc             #0

        /* SError */
        .org            0x0580
        dsb             sy
        svc             #0

        /*
         * lower EL using AArch32
         */

        /* Synchronous */
        .org            0x0600
        dsb             sy
        svc             #0

        /* IRQ */
        .org            0x0680
        dsb             sy
        svc             #0

        /* FIQ */
        .org            0x0700
        dsb             sy
        svc             #0

        /* SError */
        .org            0x0780
        dsb             sy
        svc             #0

save_rest_registers:
        stp             x3,  x4,  [sp,  #(EXC_X3 * 8)]
        stp             x5,  x6,  [sp,  #(EXC_X5 * 8)]
        stp             x7,  x8,  [sp,  #(EXC_X7 * 8)]
        stp             x9,  x10, [sp,  #(EXC_X9 * 8)]
        stp             x11, x12, [sp,  #(EXC_X11 * 8)]
        stp             x13, x14, [sp,  #(EXC_X13 * 8)]
        stp             x15, x16, [sp,  #(EXC_X15 * 8)]
        stp             x17, x18, [sp,  #(EXC_X17 * 8)]
        stp             x19, x20, [sp,  #(EXC_X19 * 8)]
        stp             x21, x22, [sp,  #(EXC_X21 * 8)]
        stp             x23, x24, [sp,  #(EXC_X23 * 8)]
        stp             x25, x26, [sp,  #(EXC_X25 * 8)]
        stp             x27, x28, [sp,  #(EXC_X27 * 8)]
        stp             x29, x30, [sp,  #(EXC_X29 * 8)]

        mrs             x1,  ESR_EL2
        mrs             x2,  SPSR_EL2
        mrs             x3,  ELR_EL2
        stp             x0,  x1,  [sp,  #(EXC_VECTOR * 8)]
        stp             x2,  x3,  [sp,  #(EXC_SPSR * 8)]

        mov             x0,  sp
        bl              exception_handler_el2
        b               .

        .end

