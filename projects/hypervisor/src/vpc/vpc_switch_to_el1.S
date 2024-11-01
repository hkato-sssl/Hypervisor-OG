// SPDX-License-Identifier: Apache-2.0
/*
 * vpc/vpc_switch_to_el1.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"
#include "driver/aarch64/system_register.h"
#include "hypervisor/thread.h"
#include "hypervisor/vpc_register.h"

        .global         vpc_switch_to_el1

        .section        ".text.vpc_switch_to_el1", "ax", %progbits
        .type           vpc_switch_to_el1, %function
        .balign         4
        /*
         * volatile void vpc_switch_to_el1(uint64_t *regs)
         */
vpc_switch_to_el1:
        // disable all interrupts

        msr             DAIFSet, #(DAIF_I | DAIF_F)

        // save current context

        str             lr,  [sp, #-16]!
        stp             x28, x29, [sp, #-16]!
        stp             x26, x27, [sp, #-16]!
        stp             x24, x25, [sp, #-16]!
        stp             x22, x23, [sp, #-16]!
        stp             x20, x21, [sp, #-16]!
        stp             x18, x19, [sp, #-16]!

        // save current sp

        mrs             x1,  TPIDR_EL2
        mov             x2,  sp
        str             x2,  [x1, #(TLS_THREAD_SP * 8)]

        // restore a system register

        ldr             x2,  [x0, #(VPC_PAR_EL1 * 8)]
        msr             PAR_EL1, x2

        // restore special purpose registers

        msr             SPSel, #0
        ldp             x2,  x3,  [x0, #(VPC_SP_EL0 * 8)]
        ldp             x4,  x5,  [x0, #(VPC_PC * 8)]
        mov             sp,  x2
        msr             SP_EL1, x3
        msr             ELR_EL2, x4
        msr             SPSR_EL2, x5

        // enable Advanced SIMD/Floating-point function

        mrs             x1, CPTR_EL2
        bic             x1, x1, #BIT(10)    // clear CPTR_EL2.TFP
        msr             CPTR_EL2, x1
        isb

        // restore general purpose registers

        ldp             x1,  x2,  [x0, #(VPC_X1 * 8)]
        ldp             x3,  x4,  [x0, #(VPC_X3 * 8)]
        ldp             x5,  x6,  [x0, #(VPC_X5 * 8)]
        ldp             x7,  x8,  [x0, #(VPC_X7 * 8)]
        ldp             x9,  x10, [x0, #(VPC_X9 * 8)]
        ldp             x11, x12, [x0, #(VPC_X11 * 8)]
        ldp             x13, x14, [x0, #(VPC_X13 * 8)]
        ldp             x15, x16, [x0, #(VPC_X15 * 8)]
        ldp             x17, x18, [x0, #(VPC_X17 * 8)]
        ldp             x19, x20, [x0, #(VPC_X19 * 8)]
        ldp             x21, x22, [x0, #(VPC_X21 * 8)]
        ldp             x23, x24, [x0, #(VPC_X23 * 8)]
        ldp             x25, x26, [x0, #(VPC_X25 * 8)]
        ldp             x27, x28, [x0, #(VPC_X27 * 8)]
        ldp             x29, x30, [x0, #(VPC_X29 * 8)]
        ldr             x0,  [x0, #(VPC_X0 * 8)]

        // switch to EL1
        
        eret

        .end

