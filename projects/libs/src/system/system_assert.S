// SPDX-License-Identifier: Apache-2.0
/*
 * system/system_assert.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         system_assert

        .section        ".text.system_assert", "ax", %progbits
        .type           system_assert, %function
        .balign         4
        /*
         *  void system_assert(const char *func, uint32_t line)
         */
system_assert:
        svc             #0xffff
        ret

        .end

