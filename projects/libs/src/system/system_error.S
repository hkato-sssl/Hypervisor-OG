// SPDX-License-Identifier: Apache-2.0
/*
 * system/system_error.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         system_error

        .section        ".text.system_error", "ax", %progbits
        .type           system_error, %function
        .balign         4
        /*
         * void system_error(const char *func, uint32_t line, const char *message)
         */
system_error:
        svc             #0xfffe
        ret

        .end

