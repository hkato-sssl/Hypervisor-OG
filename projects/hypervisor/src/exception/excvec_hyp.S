// SPDX-License-Identifier: Apache-2.0
/*
 * exception/excvec_hyp.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "driver/aarch64/exception.h"
#include "hypervisor/thread.h"
#include "hypervisor/vpc_register.h"

#define VECTOR          (0b - excvec_hyp)

        .extern         hyp_exception_handler

        .macro          exc_entry_sp0
0:
        dsb             sy
        sub             sp, sp, #(NR_EXC_REGS * 8)
        stp             x0, x1, [sp, #(EXC_X0 * 8)]
        mrs             x0, SP_EL0
        stp             lr, x0, [sp, #(EXC_LR * 8)]
        mov             x0, #VECTOR
        mrs             x1, ESR_EL2
        stp             x0, x1, [sp, #(EXC_VECTOR * 8)]
        b               store_rest_registers
        .endm

        .macro          exc_entry_spx
0:
        dsb             sy
        msr             TPIDR_EL0, x2
        mrs             x2, TPIDR_EL2
        ldr             x2, [x2, #TLS_EXCEPTION_SP * 8]
        stp             x0, x1,  [x2, #-((NR_EXC_REGS - EXC_X0) * 8)]
        mov             x0, sp
        stp             lr, x0,  [x2, #-((NR_EXC_REGS - EXC_LR) * 8)]
        mov             x0, #VECTOR
        mrs             x1, ESR_EL2
        stp             x0, x1,  [x2, #-((NR_EXC_REGS - EXC_VECTOR) * 8)]!
        mov             sp, x2
        mrs             x2, TPIDR_EL0
        b               store_rest_registers
        .endm

        .macro          hyp_exc_entry
0:
        dsb             sy
        stp             x0, x1, [sp, #-16]!
        mrs             x0, TPIDR_EL2
        mov             x1, #VECTOR
        str             x1, [x0, #(TLS_EXCEPTION_VECTOR * 8)]
        ldr             x1, [x0, #(TLS_CURRENT_VPC_REGS * 8)]
        b               store_vpc_registers
        .endm

        .global         excvec_hyp
        .section        ".text.excvec.hyp", "ax", %progbits
        .type           excvec_hyp, %function
        .balign         2048
excvec_hyp:
        /*
         * current EL with SP0
         */

        /* Synchronous */
        .org            0x0000
        exc_entry_sp0

        /* IRQ */
        .org            0x0080
        exc_entry_sp0

        /* FIQ */
        .org            0x0100
        exc_entry_sp0

        /* SError */
        .org            0x0180
        exc_entry_sp0

        /*
         * current EL with SPx
         */

        /* Synchronous */
        .org            0x0200
        exc_entry_spx

        /* IRQ */
        .org            0x0280
        exc_entry_spx

        /* FIQ */
        .org            0x0300
        exc_entry_spx

        /* SError */
        .org            0x0380
        exc_entry_spx

        /*
         * lower EL using AArch64
         */

        /* Synchronous */
        .org            0x0400
        hyp_exc_entry

        /* IRQ */
        .org            0x0480
        hyp_exc_entry

        /* FIQ */
        .org            0x0500
        hyp_exc_entry

        /* SError */
        .org            0x0580
        hyp_exc_entry

        /*
         * lower EL using AArch32
         */

        /* Synchronous */
        .org            0x0600
        hyp_exc_entry

        /* IRQ */
        .org            0x0680
        hyp_exc_entry

        /* FIQ */
        .org            0x0700
        hyp_exc_entry

        /* SError */
        .org            0x0780
        hyp_exc_entry

        .balign         32
store_rest_registers:
        stp             x2,  x3,  [sp, #(EXC_X2 * 8)]
        stp             x4,  x5,  [sp, #(EXC_X4 * 8)]
        stp             x6,  x7,  [sp, #(EXC_X6 * 8)]
        stp             x8,  x9,  [sp, #(EXC_X8 * 8)]
        stp             x10, x11, [sp, #(EXC_X10 * 8)]
        stp             x12, x13, [sp, #(EXC_X12 * 8)]
        stp             x14, x15, [sp, #(EXC_X14 * 8)]
        stp             x16, x17, [sp, #(EXC_X16 * 8)]
        stp             x18, x19, [sp, #(EXC_X18 * 8)]
        stp             x20, x21, [sp, #(EXC_X20 * 8)]
        stp             x22, x23, [sp, #(EXC_X22 * 8)]
        stp             x24, x25, [sp, #(EXC_X24 * 8)]
        stp             x26, x27, [sp, #(EXC_X26 * 8)]
        stp             x28, x29, [sp, #(EXC_X28 * 8)]

        mrs             x2, SPSR_EL2
        mrs             x3, ELR_EL2
        stp             x2, x3, [sp, #(EXC_SPSR * 8)]

        mov             x0,  sp
        bl              exception_handler_el2
        svc             #0xffff

        .balign         32
store_vpc_registers:
        stp             x2,  x3,  [x1, #(VPC_X2 * 8)]
        stp             x4,  x5,  [x1, #(VPC_X4 * 8)]
        stp             x6,  x7,  [x1, #(VPC_X6 * 8)]
        stp             x8,  x9,  [x1, #(VPC_X8 * 8)]
        stp             x10, x11, [x1, #(VPC_X10 * 8)]
        stp             x12, x13, [x1, #(VPC_X12 * 8)]
        stp             x14, x15, [x1, #(VPC_X14 * 8)]
        stp             x16, x17, [x1, #(VPC_X16 * 8)]
        stp             x18, x19, [x1, #(VPC_X18 * 8)]
        stp             x20, x21, [x1, #(VPC_X20 * 8)]
        stp             x22, x23, [x1, #(VPC_X22 * 8)]
        stp             x24, x25, [x1, #(VPC_X24 * 8)]
        stp             x26, x27, [x1, #(VPC_X26 * 8)]
        stp             x28, x29, [x1, #(VPC_X28 * 8)]
        str             x30, [x1, #(VPC_X30 * 8)]
        ldp             x2,  x3,  [sp], #16
        stp             x2,  x3,  [x1, #(VPC_X0 * 8)]
        mrs             x2,  SP_EL0
        mrs             x3,  SP_EL1
        stp             x2,  x3,  [x1, #(VPC_SP_EL0 * 8)]
        mrs             x2,  ELR_EL2
        mrs             x3,  SPSR_EL2
        stp             x2,  x3,  [x1, #(VPC_PC * 8)]

        // store system registers

        mrs             x2, PAR_EL1
        str             x2, [x1, #(VPC_PAR_EL1 * 8)]

        mrs             x2, ESR_EL2
        str             x2, [x1, #(VPC_ESR_EL2 * 8)]

        mrs             x2, HCR_EL2
        str             x2, [x1, #(VPC_HCR_EL2 * 8)]

        mrs             x2, FAR_EL2
        str             x2, [x1, #(VPC_FAR_EL2 * 8)]

        mrs             x2, HPFAR_EL2
        str             x2, [x1, #(VPC_HPFAR_EL2 * 8)]

        // disable Advanced SIMD/FLoating-point function

        mrs             x2, CPTR_EL2
        orr             x2, x2, #BIT(10)    // set CPTR_EL2.TFP
        msr             CPTR_EL2, x2
        isb

        // load the hypervisor context

        msr             SPSel, #0
        ldr             x1,  [x0, #(TLS_THREAD_SP * 8)]
        mov             sp,  x1

        ldp             x18, x19, [sp], #16
        ldp             x20, x21, [sp], #16
        ldp             x22, x23, [sp], #16
        ldp             x24, x25, [sp], #16
        ldp             x26, x27, [sp], #16
        ldp             x28, x29, [sp], #16
        ldr             lr,  [sp], #16

        // return from vpc_launch() or vpc_resume()

        mov             x0, #SUCCESS
        ret

        .end

