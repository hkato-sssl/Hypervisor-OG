// SPDX-License-Identifier: Apache-2.0
/*
 * system/stdc/string/strlen.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         strlen

        .section        ".text.strlen", "ax", %progbits
        .type           strlen, %function
        /*
         *  size_t strlen(const char *s)
         */
        .balign         4
strlen:
        mov             x1, x0
0:
        ldrb            w2, [x0], #1
        cbnz            w2, 0b

        sub             x0, x0, #1
        sub             x0, x0, x1
        ret

        .end

