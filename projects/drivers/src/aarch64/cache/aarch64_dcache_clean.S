// SPDX-License-Identifier: Apache-2.0
/*
 * aarch64/cache/aarch64_dcache_clean.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         aarch64_dcache_clean
        .global         aarch64_dcache_clean_range

        .extern         aarch64_dcache_op_range

        .section        ".text.aarch64_dcache_clean", "ax", %progbits
        .type           aarch64_dcache_clean, %function
        .balign         4
aarch64_dcache_clean:
        dsb             osh
        dc              cvac, x0
        dsb             osh
        ret

        .section        ".text.aarch64_dcache_clean_range", "ax", %progbits
        .type           aarch64_dcache_clean_range, %function
        .balign         4
aarch64_dcache_clean_range:
        adr             x2,  op
        b               aarch64_dcache_op_range
op:
        dc              cvac, x0
        ret

        .end

