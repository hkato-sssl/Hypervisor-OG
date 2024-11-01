// SPDX-License-Identifier: Apache-2.0
/*
 * aarch64/aarch64_irq.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"
#include "driver/aarch64/system_register.h"

        .global         aarch64_lock_irq
        .global         aarch64_unlock_irq
        .global         aarch64_disable_irq
        .global         aarch64_enable_irq

        .section        ".text.aarch64_lock_irq", "ax", %progbits
        .type           aarch64_lock_irq, %function
        .balign         4
aarch64_lock_irq:
        mrs             x1,  DAIF
        eor             x0,  x1,  #PSTATE_I
        msr             DAIFSet,  #DAIF_I
        ret

        .section        ".text.aarch64_unlock_irq", "ax", %progbits
        .type           aarch64_unlock_irq, %function
        .balign         4
aarch64_unlock_irq:
        cbz             x0, 0f
        msr             DAIFClr,  #DAIF_I
0:
        ret

        .section        ".text.aarch64_disable_irq", "ax", %progbits
        .type           aarch64_disable_irq, %function
        .balign         4
aarch64_disable_irq:
        msr             DAIFSet,  #DAIF_I
        ret

        .section        ".text.aarch64_enable_irq", "ax", %progbits
        .type           aarch64_enable_irq, %function
        .balign         4
aarch64_enable_irq:
        msr             DAIFClr,  #DAIF_I
        ret





        .end

