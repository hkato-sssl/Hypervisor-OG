// SPDX-License-Identifier: Apache-2.0
/*
 * aarch64/psci/aarch64_psci.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"

        .global         aarch64_psci

        .section        ".text.aarch64_psci", "ax", %progbits
        .type           aarch64_psci, %function
        .balign         4
        /*
         * int32_t aarch64_psci(uint64_t request[4])
         */
aarch64_psci:
        ldp             x2, x3, [x0, #16]
        ldp             x0, x1, [x0]
        smc             #0
        ret

        .end

