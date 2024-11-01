// SPDX-License-Identifier: Apache-2.0
/*
 * lib/aarch64/asm.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef LIB_AARCH64_ASM_H
#define LIB_AARCH64_ASM_H

/* includes */

/* defines */

#ifdef ASSEMBLY
.macro      BR_EL, table
        adr         ip0, \table
        mrs         ip1, CurrentEL
        add         ip0, ip0, ip1, lsl #1
        ldr         ip1, [ip0]
        br          ip1
        .balign      8
        .endm

        .macro mov32, reg, val
        movz        \reg, #:abs_g0_nc:\val
        movk        \reg, #:abs_g1:\val
        .endm

        .macro mov64, reg, val
        movz        \reg, #:abs_g0_nc:\val
        movk        \reg, #:abs_g1_nc:\val
        movk        \reg, #:abs_g2_nc:\val
        movk        \reg, #:abs_g3:\val
        .endm

        .macro adrl, reg, val
        adrp        \reg, \val
        add         \reg, \reg, #:lo12:\val
        .endm
#endif /* ASSEMBLY */

#endif /* LIB_AARCH64_ASM_H */
