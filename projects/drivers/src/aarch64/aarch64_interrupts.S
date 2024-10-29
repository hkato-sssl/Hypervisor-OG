// SPDX-License-Identifier: Apache-2.0
/*
 * aarch64/aarch64_interrupts.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"
#include "driver/aarch64/system_register.h"

        .global         aarch64_lock_interrupts
        .global         aarch64_unlock_interrupts
        .global         aarch64_disable_interrupts
        .global         aarch64_enable_interrupts

        .section        ".text.aarch64_lock_interrupts", "ax", %progbits
        .type           aarch64_lock_interrupts, %function
        .balign         4
aarch64_lock_interrupts:
        mrs             x1,  DAIF
        eor             x0,  x1,  #(PSTATE_I | PSTATE_F)
        and             x1,  x1,  #(PSTATE_I | PSTATE_F)
        msr             DAIFset,  #(DAIF_I | DAIF_F)
        ret

        .section        ".text.aarch64_unlock_interrupts", "ax", %progbits
        .type           aarch64_unlock_interrupts, %function
        .balign         4
aarch64_unlock_interrupts:
        cbz             x0,  0f
        mrs             x1,  DAIF
        bic             x2,  x1,  x0
        msr             DAIF, x2
0:
        ret

        .section        ".text.aarch64_disable_interrupts", "ax", %progbits
        .type           aarch64_disable_interrupts, %function
        .balign         4
aarch64_disable_interrupts:
        msr             DAIFSet,  #(DAIF_I | DAIF_F)
        ret

        .section        ".text.aarch64_enable_interrupts", "ax", %progbits
        .type           aarch64_enable_interrupts, %function
        .balign         4
aarch64_enable_interrupts:
        msr             DAIFClr,  #(DAIF_I | DAIF_F)
        ret

        .end

