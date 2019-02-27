/*
 * aarch64/cache/aarch64_icache_invd.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"
#include "lib/system/errno.h"

        .global         aarch64_icache_invd_all
        .global         aarch64_icache_invd
        .global         aarch64_icache_invd_range

        .section        ".text.aarch64_icache_invd_all", "ax", %progbits
        .type           aarch64_icache_invd_all, %function
        .balign         4
aarch46_icache_invd_all:
        dsb             ish
        ic              ialluis
        dsb             ish
        isb
        ret

        .section        ".text.aarch64_icache_invd", "ax", %progbits
        .type           aarch64_icache_invd, %function
        .balign         4
aarch46_icache_invd:
        dsb             ish
        ic              ivau, x0
        dsb             ish
        isb
        ret

        .end

