/*
 * system/stdc/string/memset.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         memset

        .section        ".text.memset", "ax", %progbits
        .type           memset, %function
        /*
         * void *memset(void *s, int c, size_t n)
         */
        .balign         4
memset:
        cbz             x2, 999f
        mov             x3, x0
0:
        strb            w1, [x3], #1
        subs            x2, x2, #1
        b.ne            0b
999:
        ret

        .end

