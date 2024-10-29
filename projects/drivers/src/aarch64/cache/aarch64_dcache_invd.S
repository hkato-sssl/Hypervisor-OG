// SPDX-License-Identifier: Apache-2.0
/*
 * aarch64/cache/aarch64_dcache_invd.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         aarch64_dcache_invd
        .global         aarch64_dcache_invd_range

        .extern         aarch64_dcache_op_range

        .section        ".text.aarch64_dcache_invd", "ax", %progbits
        .type           aarch64_dcache_invd, %function
        .balign         4
aarch64_dcache_invd:
        dsb             osh
        dc              ivac, x0
        dsb             osh
        ret

        .section        ".text.aarch64_dcache_invd_range", "ax", %progbits
        .type           aarch64_dcache_invd_range, %function
        .balign         4
aarch64_dcache_invd_range:
        adr             x2,  op
        b               aarch64_dcache_op_range
op:
        dc              ivac, x0
        ret

        .end
