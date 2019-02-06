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
        stp             x29, x30, [sp,  #-16]!
        mov             x30, #0x0000
        b               save_rest_registers

        /* IRQ */
        .org            0x0080
        stp             x29, x30, [sp,  #-16]!
        mov             x30, #0x0080
        b               save_rest_registers

        /* FIQ */
        .org            0x0100
        stp             x29, x30, [sp,  #-16]!
        mov             x30, #0x0100
        b               save_rest_registers

        /* SError */
        .org            0x0180
        stp             x29, x30, [sp,  #-16]!
        mov             x30, #0x0180
        b               save_rest_registers

        /*
         * current EL with SPx
         */

        /* Synchronous */
        .org            0x0200
        stp             x29, x30, [sp,  #-16]!
        mov             x30, #0x0200
        b               save_rest_registers

        /* IRQ */
        .org            0x0280
        stp             x29, x30, [sp,  #-16]!
        mov             x30, #0x0280
        b               save_rest_registers

        /* FIQ */
        .org            0x0300
        stp             x29, x30, [sp,  #-16]!
        mov             x30, #0x0300
        b               save_rest_registers

        /* SError */
        .org            0x0380
        stp             x29, x30, [sp,  #-16]!
        mov             x30, #0x0380
        b               save_rest_registers

        /*
         * lower EL using AArch64
         */

        /* Synchronous */
        .org            0x0400
        stp             x29, x30, [sp,  #-16]!
        mov             x30, #0x0400
        b               save_rest_registers

        /* IRQ */
        .org            0x0480
        stp             x29, x30, [sp,  #-16]!
        mov             x30, #0x0480
        b               save_rest_registers

        /* FIQ */
        .org            0x0500
        stp             x29, x30, [sp,  #-16]!
        mov             x30, #0x0500
        b               save_rest_registers

        /* SError */
        .org            0x0580
        stp             x29, x30, [sp,  #-16]!
        mov             x30, #0x0580
        b               save_rest_registers

        /*
         * lower EL using AArch32
         */

        /* Synchronous */
        .org            0x0600
        stp             x29, x30, [sp,  #-16]!
        mov             x30, #0x0600
        b               save_rest_registers

        /* IRQ */
        .org            0x0680
        stp             x29, x30, [sp,  #-16]!
        mov             x30, #0x0680
        b               save_rest_registers

        /* FIQ */
        .org            0x0700
        stp             x29, x30, [sp,  #-16]!
        mov             x30, #0x0700
        b               save_rest_registers

        /* SError */
        .org            0x0780
        stp             x29, x30, [sp,  #-16]!
        mov             x30, #0x0780
        b               save_rest_registers

save_rest_registers:
        stp             x27, x28, [sp,  #-16]!
        stp             x25, x26, [sp,  #-16]!
        stp             x23, x24, [sp,  #-16]!
        stp             x21, x22, [sp,  #-16]!
        stp             x19, x20, [sp,  #-16]!
        stp             x17, x18, [sp,  #-16]!
        stp             x15, x16, [sp,  #-16]!
        stp             x13, x14, [sp,  #-16]!
        stp             x11, x12, [sp,  #-16]!
        stp             x9,  x10, [sp,  #-16]!
        stp             x7,  x8,  [sp,  #-16]!
        stp             x5,  x6,  [sp,  #-16]!
        stp             x3,  x4,  [sp,  #-16]!
        stp             x1,  x2,  [sp,  #-16]!

        mrs             x1,  ESR_EL2
        mrs             x2,  IFSR32_EL2
        mrs             x3,  FAR_EL2
        mrs             x4,  SPSR_EL2
        mrs             x5,  ELR_EL2
        stp             x4,  x5,  [sp,  #-32]!
        stp             x2,  x3,  [sp,  #-16]!
        stp             x30, x1,  [sp,  #-16]!
        add             x1,  sp,  #(NR_EXC_REGS * 8)
        stp             x1,  x0,  [sp,  #(EXC_SP * 8)]

        mov             x0,  sp
        bl              exception_entry_el2
        b               .

        .end

