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

        .macro mov32, reg, val
        movz \reg, #:abs_g0_nc:\val
        movk \reg, #:abs_g1:\val
        .endm

#endif /* ASSEMBLY */

#endif /* LIB_AARCH64_ASM_H */

