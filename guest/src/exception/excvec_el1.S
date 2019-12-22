/*
 * exception/excvec_el1.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include "lib/asm.h"
#include "lib/bit.h"
#include "driver/aarch64/exception.h"

        .extern         exception_handler_el1

        .macro          exc_entry_sp0, vector
        dsb             sy
        stp             x0, x1,  [sp, #-((NR_EXC_REGS - EXC_X0) * 8)]
        mrs             x0, SP_EL0
        stp             lr, x0,  [sp, #-((NR_EXC_REGS - EXC_LR) * 8)]
        mov             x0, #\vector
        mrs             x1, ESR_EL1
        stp             x0, x1,  [sp, #-((NR_EXC_REGS - EXC_VECTOR) * 8)]!
        b               store_rest_registers
        .endm

        .macro          exc_entry_spx, vector
        dsb             sy
        msr             TPIDR_EL0, x2
        mrs             x2, TPIDR_EL1
        stp             x0, x1,  [x2, #-((NR_EXC_REGS - EXC_X0) * 8)]
        mov             x0, sp
        stp             lr, x0,  [x2, #-((NR_EXC_REGS - EXC_LR) * 8)]
        mov             x0, #\vector
        mrs             x1, ESR_EL1
        stp             x0, x1,  [sp, #-((NR_EXC_REGS - EXC_VECTOR) * 8)]!
        b               store_rest_registers
        .endm

        .macro          intr_entry, vector
        msr             SPSel, #0
        stp             x0, x1, [sp, #-16]!
        mov             x0, #\vector
        b               interrupt_operation
        .endm

        .global         excvec_el1
        .section        ".text.excvec.el1", "ax", %progbits
        .type           excvec_el1, %function
        .balign         2048
excvec_el1:
        /*
         * current EL with SP0
         */

        /* Synchronous */
        .org            0x0000
        exc_entry_sp0   0x0000

        /* IRQ */
        .org            0x0080
        intr_entry      0x0080

        /* FIQ */
        .org            0x0100
        intr_entry      0x0100

        /* SError */
        .org            0x0180
        exc_entry_sp0   0x0180

        /*
         * current EL with SPx
         */

        /* Synchronous */
        .org            0x0200
        exc_entry_spx   0x0200

        /* IRQ */
        .org            0x0280
        exc_entry_spx   0x0280

        /* FIQ */
        .org            0x0300
        exc_entry_spx   0x0300

        /* SError */
        .org            0x0380
        exc_entry_spx   0x0380

        /*
         * lower EL using AArch64
         */

        /* Synchronous */
        .org            0x0400
        exc_entry_spx   0x0400

        /* IRQ */
        .org            0x0480
        exc_entry_spx   0x0480

        /* FIQ */
        .org            0x0500
        exc_entry_spx   0x0500

        /* SError */
        .org            0x0580
        exc_entry_spx   0x0580

        /*
         * lower EL using AArch32
         */

        /* Synchronous */
        .org            0x0600
        exc_entry_spx   0x0600

        /* IRQ */
        .org            0x0680
        exc_entry_spx   0x0680

        /* FIQ */
        .org            0x0700
        exc_entry_spx   0x0700

        /* SError */
        .org            0x0780
        exc_entry_spx   0x0780

        .balign         32
store_rest_registers:
        stp             x2,   x3,   [sp, #(EXC_X2 * 8)]
        stp             x4,   x5,   [sp, #(EXC_X4 * 8)]
        stp             x6,   x7,   [sp, #(EXC_X6 * 8)]
        stp             x8,   x9,   [sp, #(EXC_X8 * 8)]
        stp             x10,  x11,  [sp, #(EXC_X10 * 8)]
        stp             x12,  x13,  [sp, #(EXC_X12 * 8)]
        stp             x14,  x15,  [sp, #(EXC_X14 * 8)]
        stp             x16,  x17,  [sp, #(EXC_X16 * 8)]
        stp             x18,  x19,  [sp, #(EXC_X18 * 8)]
        stp             x20,  x21,  [sp, #(EXC_X20 * 8)]
        stp             x22,  x23,  [sp, #(EXC_X22 * 8)]
        stp             x24,  x25,  [sp, #(EXC_X24 * 8)]
        stp             x26,  x27,  [sp, #(EXC_X26 * 8)]
        stp             x28,  x29,  [sp, #(EXC_X28 * 8)]

        mrs             x2, SPSR_EL1
        mrs             x3, ELR_EL1
        stp             x2, x3, [sp, #(EXC_SPSR * 8)]

        mov             x0,  sp
        bl              exception_handler_el1
        svc             #0xffff

        .balign         32
interrupt_operation:
        stp             x2,  x3,  [sp, #-16]!
        stp             x4,  x5,  [sp, #-16]!
        stp             x6,  x7,  [sp, #-16]!
        stp             x8,  x9,  [sp, #-16]!
        stp             x10, x11, [sp, #-16]!
        stp             x12, x13, [sp, #-16]!
        stp             x14, x15, [sp, #-16]!
        stp             x16, x17, [sp, #-16]!
        stp             x18, x19, [sp, #-16]!
        stp             x20, x21, [sp, #-16]!
        stp             x22, x23, [sp, #-16]!
        stp             x24, x25, [sp, #-16]!
        stp             x26, x27, [sp, #-16]!
        stp             x28, x29, [sp, #-16]!
        str             x30, [sp, #-16]!
        mrs             x1,  SPSR_EL1
        mrs             x2,  ELR_EL1
        stp             x1,  x2,  [sp, #-16]!

        bl               interrupt_handler_el1

        /* return from an interrupt handler */

        ldp             x0,  x1,  [sp], #16
        msr             SPSR_EL1, x0
        msr             ELR_EL1, x1
        ldr             x30, [sp], #16
        ldp             x28, x29, [sp], #16
        ldp             x26, x27, [sp], #16
        ldp             x24, x25, [sp], #16
        ldp             x22, x23, [sp], #16
        ldp             x20, x21, [sp], #16
        ldp             x18, x19, [sp], #16
        ldp             x16, x17, [sp], #16
        ldp             x14, x15, [sp], #16
        ldp             x12, x13, [sp], #16
        ldp             x10, x11, [sp], #16
        ldp             x8,  x9,  [sp], #16
        ldp             x6,  x7,  [sp], #16
        ldp             x4,  x5,  [sp], #16
        ldp             x2,  x3,  [sp], #16
        ldp             x0,  x1,  [sp], #16
        
        eret

        .end

