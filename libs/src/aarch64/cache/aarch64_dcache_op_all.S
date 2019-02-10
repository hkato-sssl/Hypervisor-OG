/*
 * aarch64/cache/aarch64_dcache_op_all.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         aarch64_dcache_invd_all
        .global         aarch64_dcache_clean_all
        .global         aarch64_dcache_clean_invd_all

        .section        ".text.aarch64_dcache_invd_all", "ax", %progbits
        .type           aarch64_dcache_invd_all, %function
        .balign         4
aarch64_dcache_invd_all:
        adr             x0,  op_invd
        b               aarch64_dcache_op_all

op_invd:
        dc              isw, x0
        ret

        .section        ".text.aarch64_dcache_clean_all", "ax", %progbits
        .type           aarch64_dcache_clean_all, %function
        .balign         4
aarch64_dcache_clean_all:
        adr             x0, op_clean
        b               aarch64_dcache_op_all

op_clean:
        dc              csw, x0
        ret

        .section        ".text.aarch64_dcache_clean_invd_all", "ax", %progbits
        .type           aarch64_dcache_clean_invd_all, %function
        .balign         4
aarch64_dcache_clean_invd_all:
        adr             x0, op_clean_invd
        b               aarch64_dcache_op_all

op_clean_invd:
        dc              cisw, x0
        ret

        .section        ".text.aarch64_dcache_op_all", "ax", %progbits
        .type           aarch64_dcache_op_all, %function
        .balign         4
        /*
         * x0 - the maintenance function
         */
aarch64_dcache_op_all:
        mov             ip0, x0
        mov             ip1, lr

        // ip0 - the maintenance function
        // ip1 - old LR

        mrs             x9,  CLIDR_EL1
        lsl             x10, x9,  #(24 - 1)
        and             x10, x10,  #0x0e
        cbz             x10, exit

        mov             x1,  #(0 << 1)

        // x1  - (index of cache line) << 1
        // x9  - CLIDR_EL1
        // x10 - LoC << 1
loop_level:
        msr             CSSELR_EL1, x1
        isb
        mrs             x11, CCSIDR_EL1
        and             x12, x11, #7
        add             x12, x12, #4
        lsr             w13, w11, #3
        lsr             x14, x11, #32

        // x11 - CSSIDR_EL1
        // x12 - log2(number of bytes in cache line)
        // x13 - (aasociativity of cache) - 1
        // x14 - (numner of sets) - 1

        clz             w0,  w13
        mov             x15, #32
        sub             x15, x15, x0

        // x15  - A = log2(ASSOSIATIVITY)

        mov             x2,  xzr
loop_set:
        mov             x3,  xzr
loop_way:
        lsl             x0,  x2,  x15
        lsl             x4,  x1,  x12
        orr             x0,  x0,  x4
        orr             x0,  x0,  x1
        bl              ip0
        add             x3,  x3,  #1
        cmp             x13, x3
        bcs             loop_way

        add             x2,  x2,  #1
        cmp             x14, x2
        bcs             loop_set

        add             x1,  x1,  #(1 << 1)
        cmp             x10, x1
        bcs             loop_level
exit:
        br              ip1
        ret

        .end

