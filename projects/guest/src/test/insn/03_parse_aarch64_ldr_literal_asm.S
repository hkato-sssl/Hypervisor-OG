// SPDX-License-Identifier: Apache-2.0
/*
 * test/insn/03_parse_aarch64_ldr_literal_asm.S
 *
 * (C) 2020 Hidekazu Kato
 */

#include "config/system.h"
#include "lib/asm.h"

        .global         test_insn_03_ldr
        .global         test_insn_03_ldrw

        .section        ".text.test_insn_03_ldr", "ax", %progbits
        .type           test_insn_03_ldr, %function
        .balign         4
test_insn_03_ldr:       
        ldr             x0, 4096
        ldr             x1, 4096
        ldr             x2, 4096
        ldr             x3, 4096
        ldr             x4, 4096
        ldr             x5, 4096
        ldr             x6, 4096
        ldr             x7, 4096
        ldr             x8, 4096
        ldr             x9, 4096
        ldr             x10, 4096
        ldr             x11, 4096
        ldr             x12, 4096
        ldr             x13, 4096
        ldr             x14, 4096
        ldr             x15, 4096
        ldr             x16, 4096
        ldr             x17, 4096
        ldr             x18, 4096
        ldr             x19, 4096
        ldr             x20, 4096
        ldr             x21, 4096
        ldr             x22, 4096
        ldr             x23, 4096
        ldr             x24, 4096
        ldr             x25, 4096
        ldr             x26, 4096
        ldr             x27, 4096
        ldr             x28, 4096
        ldr             x29, 4096
        ldr             x30, 4096
        ldr             xzr, 4096

        .section        ".text.test_insn_03_ldrw", "ax", %progbits
        .type           test_insn_03_ldrw, %function
        .balign         4
test_insn_03_ldrw:       
        ldr             w0, 4096
        ldr             w1, 4096
        ldr             w2, 4096
        ldr             w3, 4096
        ldr             w4, 4096
        ldr             w5, 4096
        ldr             w6, 4096
        ldr             w7, 4096
        ldr             w8, 4096
        ldr             w9, 4096
        ldr             w10, 4096
        ldr             w11, 4096
        ldr             w12, 4096
        ldr             w13, 4096
        ldr             w14, 4096
        ldr             w15, 4096
        ldr             w16, 4096
        ldr             w17, 4096
        ldr             w18, 4096
        ldr             w19, 4096
        ldr             w20, 4096
        ldr             w21, 4096
        ldr             w22, 4096
        ldr             w23, 4096
        ldr             w24, 4096
        ldr             w25, 4096
        ldr             w26, 4096
        ldr             w27, 4096
        ldr             w28, 4096
        ldr             w29, 4096
        ldr             w30, 4096
        ldr             wzr, 4096

        .end

