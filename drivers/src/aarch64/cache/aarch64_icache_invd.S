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


        .section        ".text.aarch64_icache_invd_range", "ax", %progbits
        .type           aarch64_icache_invd_range, %function
        .balign         4
aarch64_icache_invd_range:
        dsb             ish

        cmp             x1,  x0         // if (x0 > x1)
        bmi             error_einval    //   return -EINVAL

        mrs             x2,  CLIDR_EL1
        lsr             x2,  x2,  #(21 - 1)
        and             x2,  x2,  #0x0e
        cbz             x2,  exit
        orr             x2,  x2,  #1
        msr             CSSELR_EL1, x2
        isb
        mrs             x3,  CCSIDR_EL1
        and             x3,  x3,  #7
        add             x3,  x3,  #4
        mov             x2,  #1
        lsl             x2,  x2,  x3
        sub             x3,  x2,  #1
        bic             x0,  x0,  x3
        bic             x1,  x1,  x3

invd_loop:
        ic              ivau,  x0
        cmp             x0,  x1
        add             x0,  x0,  x2
        bne             invd_loop

exit:
        dsb             ish
        isb
        mov             x0,  xzr
        ret
        
error_einval:
        mov             x0,  #-EINVAL
        ret

        .end

