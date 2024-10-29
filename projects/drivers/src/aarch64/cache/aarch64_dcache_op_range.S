// SPDX-License-Identifier: Apache-2.0
/*
 * aarch64/cache/aarch64_dcache_op_range.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"
#include "lib/system/errno.h"

        .global         aarch64_dcache_op_range

        .extern         aarch64_poc_cache_line_sz

        .section        ".text.aarch64_dcache_op_range", "ax", %progbits
        .type           aarch64_dcache_op_range, %function
        .balign         4
        // x0 : start address
        // x1 : size
        // x2 : an address of the maintenance function
aarch64_dcache_op_range:
        cbz             x1,  error

        mov             ip1, lr         // save original LR

        // read D-$ line size at PoC

        mrs             x3,  CLIDR_EL1
        lsr             x3,  x3,  #(24 - 1)
        and             x3,  x3,  #0x0e
        cbz             x3, exit

        sub             x3,  x3,  #(1 << 1)
        msr             CSSELR_EL1, x3
        isb
        mrs             x4,  CCSIDR_EL1
        and             x4,  x4,  #7
        add             x4,  x4,  #4
        mov             x3,  #1
        lsl             x3,  x3,  x4

        // x0 - start address
        // x1 - size
        // x2 : an address of the maintenance function
        // x3 - cache line size at PoC

        add             x1,  x0,  x1
        sub             x1,  x1,  #1

        sub             x4,  x3,  #1
        bic             x0,  x0,  x4
        bic             x1,  x1,  x4

        dsb             osh
loop:
        blr             x2
        cmp             x0,  x1
        add             x0,  x0,  x3
        bne             loop

        dsb             osh
exit:
        mov             x0,  xzr
        br              ip1

error:
        mov             x0,  #-EINVAL
        ret

        .end

