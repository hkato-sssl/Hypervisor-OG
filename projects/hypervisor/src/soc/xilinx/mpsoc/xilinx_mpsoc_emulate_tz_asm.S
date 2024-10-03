/*
 * soc/xilinx/mpsoc/xilinx_mpsoc_emulate_tz_asm.S
 *
 * (C) 2020 Hidekazu Kato
 */

#include "lib/asm.h"
#include "hypervisor/vpc_register.h"

        .global         xilinx_mpsoc_emulate_tz_asm

        .section        ".text.xilinx_mpsoc_emulate_tz_asm", "ax", %progbits
        .type           xilinx_mpsoc_emulate_tz_asm, %function
        .balign         4
        /*
         * Relaying requests to Secure-mode.
         */
xilinx_mpsoc_emulate_tz_asm:
        stp             x0,  lr,  [sp, #-16]!

        ldp             x2,  x3,  [x0, #(VPC_X2 * 8)]
        ldp             x4,  x5,  [x0, #(VPC_X4 * 8)]
        ldp             x6,  x7,  [x0, #(VPC_X6 * 8)]
        ldp             x8,  x9,  [x0, #(VPC_X8 * 8)]
        ldp             x10, x11, [x0, #(VPC_X10 * 8)]
        ldp             x12, x13, [x0, #(VPC_X12 * 8)]
        ldp             x14, x15, [x0, #(VPC_X14 * 8)]
        ldp             x16, x17, [x0, #(VPC_X16 * 8)]
        ldp             x0,  x1,  [x0, #(VPC_X0 * 8)]

        smc             #0

        ldp             x18,  lr, [sp], #16
        stp             x0,   x1, [x18, #(VPC_X0 * 8)]
        stp             x2,   x3, [x18, #(VPC_X2 * 8)]
        stp             x4,   x5, [x18, #(VPC_X4 * 8)]
        stp             x6,   x7, [x18, #(VPC_X6 * 8)]
        stp             x8,   x9, [x18, #(VPC_X8 * 8)]
        stp             x10, x11, [x18, #(VPC_X10 * 8)]
        stp             x12, x13, [x18, #(VPC_X12 * 8)]
        stp             x14, x15, [x18, #(VPC_X14 * 8)]
        stp             x16, x17, [x18, #(VPC_X16 * 8)]

        mov             x0, xzr     /* always return success */
        ret

        .end

