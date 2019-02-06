/*
 * exception/excvec_el2.S
 *
 * (C) 2019 Hidekazu Kato
 */

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
        sub             sp,  sp,  #(NR_EXC_REGS * 8)
        stp             x0,  x1,  [sp,  #(EXC_X0 * 8)]
        mrs             x0,  SP_EL0
        add             x1,  sp,  #(NR_EXC_REGS * 8)
        mov             x0,  #0x0000
        str             x0,  [sp,  #(EXC_VECTOR * 8)]
        b               save_registers

        /* IRQ */
        .org            0x0080
        sub             sp,  sp,  #(NR_EXC_REGS * 8)
        stp             x0,  x1,  [sp,  #(EXC_X0 * 8)]
        mrs             x0,  SP_EL0
        add             x1,  sp,  #(NR_EXC_REGS * 8)
        mov             x0,  #0x0080
        str             x0,  [sp,  #(EXC_VECTOR * 8)]
        b               save_registers

        /* FIQ */
        .org            0x0100
        sub             sp,  sp,  #(NR_EXC_REGS * 8)
        stp             x0,  x1,  [sp,  #(EXC_X0 * 8)]
        mrs             x0,  SP_EL0
        add             x1,  sp,  #(NR_EXC_REGS * 8)
        mov             x0,  #0x0100
        str             x0,  [sp,  #(EXC_VECTOR * 8)]
        b               save_registers

        /* SError */
        .org            0x0180
        sub             sp,  sp,  #(NR_EXC_REGS * 8)
        stp             x0,  x1,  [sp,  #(EXC_X0 * 8)]
        mrs             x0,  SP_EL0
        add             x1,  sp,  #(NR_EXC_REGS * 8)
        mov             x0,  #0x0180
        str             x0,  [sp,  #(EXC_VECTOR * 8)]
        b               save_registers

        /*
         * current EL with SPx
         */

        /* Synchronous */
        .org            0x0200
        b               .

        /* IRQ */
        .org            0x0280
        b               .

        /* FIQ */
        .org            0x0300
        b               .

        /* SError */
        .org            0x0380
        b               .

        /*
         * lower EL using AArch64
         */

        /* Synchronous */
        .org            0x0400
        b               .

        /* IRQ */
        .org            0x0480
        b               .

        /* FIQ */
        .org            0x0500
        b               .

        /* SError */
        .org            0x0580
        b               .

        /*
         * lower EL using AArch32
         */

        /* Synchronous */
        .org            0x0600
        b               .

        /* IRQ */
        .org            0x0680
        b               .

        /* FIQ */
        .org            0x0700
        b               .

        /* SError */
        .org            0x0780
        b               .

        /* sp - the base address of a register array */
save_registers:
        mrs             x0,  ESR_EL2
        str             x0,  [sp,  #(EXC_ESR * 8)]
        mrs             x0,  IFSR32_EL2
        mrs             x1,  FAR_EL2
        stp             x0,  x1,  [sp,  #(EXC_IFSR32 * 8)]
        mrs             x0,  SPSR_EL2
        mrs             x1,  ELR_EL2
        stp             x0,  x1,  [sp,  #(EXC_SPSR * 2)]

        stp             x4,  x5,  [sp,  #(EXC_X4 * 8)]
        stp             x6,  x7,  [sp,  #(EXC_X6 * 8)]
        stp             x8,  x9,  [sp,  #(EXC_X8 * 8)]
        stp             x10, x11, [sp,  #(EXC_X10 * 8)]
        stp             x12, x13, [sp,  #(EXC_X12 * 8)]
        stp             x14, x15, [sp,  #(EXC_X14 * 8)]
        stp             x16, x17, [sp,  #(EXC_X16 * 8)]
        stp             x18, x19, [sp,  #(EXC_X18 * 8)]
        stp             x20, x21, [sp,  #(EXC_X20 * 8)]
        stp             x22, x23, [sp,  #(EXC_X22 * 8)]
        stp             x24, x25, [sp,  #(EXC_X24 * 8)]
        stp             x26, x27, [sp,  #(EXC_X26 * 8)]
        stp             x28, x29, [sp,  #(EXC_X28 * 8)]
        str             x30, [sp,  #(EXC_X30 * 8)]

        mov             x0,  sp
        bl              exception_entry_el2
        b               .

        .end

