/*
 * aarch64/aarch64_interrupts.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"
#include "driver/aarch64/system_register.h"

#define MASK_BITS       (PSTATE_I | PSTATE_F)

        .global         aarch64_lock_interrupts
        .global         aarch64_unlock_interrupts

        .section        ".text.aarch64_lock_interrupts", "ax", %progbits
        .type           aarch64_lock_interrupts, %function
        .balign         4
aarch64_lock_interrupts:
        mrs             x1,  DAIF
        mvn             x0,  x1
        and             x0,  x0,  #MASK_BITS
        orr             x2,  x1,  #MASK_BITS
        msr             DAIF, x2
        ret

        .section        ".text.aarch64_unlock_interrupts", "ax", %progbits
        .type           aarch64_unlock_interrupts, %function
        .balign         4
aarch64_unlock_interrupts:
        mrs             x1,  DAIF
        bic             x2,  x0,  x1
        msr             DAIF, x2
        ret

        .end

