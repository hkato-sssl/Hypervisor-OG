/*
 * system/stdc/string/memcpy.S
 *
 * (C) 2020 Hidekazu Kato
 */
        .global         memcpy

        .section        ".text.memcpy", "ax", %progbits
        .type           memcpy, %function
        /*
         * void *memcpy(void *dst, const void *src, size_t n);
         */
        .balign         4
memcpy:
        cbz             x2, 999f
        mov             x3, x0
0:
        ldrb            w4, [x1], #1
        strb            w4, [x3], #1
        subs            x2, x2, #1
        bne             0b
999:
        ret

        .end

