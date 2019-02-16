/*
 * aarch64/cache/aarch64_dcache_clean.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         aarch64_dcache_clean
        .global         aarch64_dcache_clean_range

        .extern         aarch64_poc_cache_line_sz

        .section        ".text.aarch64_dcache_clean", "ax", %progbits
        .type           aarch64_dcache_clean, %function
        .balign         4
aarch64_dcache_clean:
        dc              cvac, x0
        dsb             osh
        ret

        .section        ".text.aarch64_dcache_clean_range", "ax", %progbits
        .type           aarch64_dcache_clean_range, %function
        .balign         4
aarch64_dcache_clean_range:
        str             lr,  [sp,  #-16]!
        stp             x0,  x1,  [sp,  #-16]!
        bl              aarch64_poc_line_sz
        ldp             x1,  x2,  [sp],  #16
        ldr             lr,  [sp],  #16
        cbz             x0,  exit

        // x0 - cache line size at PoC
        // x1 - start address
        // x2 - end address

        sub             x3,  x0,  #1
        and             x1,  x1,  x3
        and             x2,  x2,  x3

clean_loop:
        dc              cvac, x1
        cmp             x1,  x2
        add             x1,  x1,  x0
        bne             clean_loop

        dsb             osh
exit:
        ret

        .end

