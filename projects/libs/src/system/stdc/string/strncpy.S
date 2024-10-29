// SPDX-License-Identifier: Apache-2.0
/*
 * system/stdc/string/strncpy.S
 *
 * (C) 2020 Hidekazu Kato
 */
        .global         strncpy

        .section        ".text.strncpy", "ax", %progbits
        .type           strncpy, %function
        /*
         * char *strncpy(char *dst, const char *src, size_t n);
         */
        .balign         4
strncpy:
        cbz             x2, 999f
        mov             x3, x0
0:
        ldrb            w4, [x1], #1
        strb            w4, [x3], #1
        cbz             w4, 999f

        subs            x2, x2, #1
        bne             0b
999:
        ret

        .end

