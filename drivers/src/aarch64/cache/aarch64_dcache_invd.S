/*
 * aarch64/cache/aarch64_dcache_invd.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         aarch64_dcache_invd
        .global         aarch64_dcache_invd_range

        .extern         aarch64_poc_cache_line_sz

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
        dsb             osh

        str             lr,  [sp,  #-16]!
        stp             x0,  x1,  [sp,  #-16]!
        bl              aarch64_poc_line_sz
        ldp             x1,  x2,  [sp],  #16
        ldr             lr,  [sp],  #16
        cbz             x0,  exit

        // x0 - cache line size at PoC
        // x1 - start address
        // x2 - size

		add				x2,  x2,  x1
		sub				x2,  x2,  #1

        sub             x3,  x0,  #1
        bic             x1,  x1,  x3
        bic             x2,  x2,  x3

invd_loop:
        dc              ivac, x1
        cmp             x1,  x2
        add             x1,  x1,  x0
        bne             invd_loop

        dsb             osh
exit:
        ret

        .end

