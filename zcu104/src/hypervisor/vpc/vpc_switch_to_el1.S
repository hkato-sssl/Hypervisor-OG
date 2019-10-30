/*
 * vpc/vpc_switch_to_el1.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"

#include "lib/asm.h"
#include "driver/aarch64/system_register.h"
#include "hypervisor/vpc_register.h"

        .global         vpc_switch_to_el1

        .section        ".text.vpc_switch_to_el1", "ax", %progbits
        .type           vpc_switch_to_el1, %function
        .balign         4
        /*
         * volatile void vpc_switch_to_el1(uint64_t *regs)
         */
vpc_switch_to_el1:
        // disable all interrupts, and select SP0

        msr             DAIFSet, #(DAIF_I | DAIF_F)
        msr             SPSel, #0

        // save the address of current register array

        msr             TPIDR_EL2, x0

        // restore special purpose registers

        ldp             x2, x3, [x0, #(VPC_SP_EL0 * 8)]
        ldp             x4, x5, [x0, #(VPC_PC * 8)]
        mov             sp, x2
        msr             SP_EL1, x3
        msr             ELR_EL2, x4
        msr             SPSR_EL2, x5

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
        ldr             x0, [x0, #(VPC_X0 * 8)]

        // switch to EL1
        
        eret

        .end

