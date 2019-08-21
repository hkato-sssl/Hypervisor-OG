/*
 * aarch64/aarch64_fiq.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"
#include "driver/aarch64/system_register.h"

        .global         aarch64_lock_fiq
        .global         aarch64_unlock_fiq

        .section        ".text.aarch64_lock_fiq", "ax", %progbits
        .type           aarch64_lock_fiq, %function
        .balign         4
aarch64_lock_fiq:
        mrs             x1,  DAIF
        eor             x0,  x1,  #PSTATE_F
        msr             DAIFSet,  #DAIF_F
        ret

        .section        ".text.aarch64_unlock_fiq", "ax", %progbits
        .type           aarch64_unlock_fiq, %function
        .balign         4
aarch64_unlock_fiq:
        cbz             x0, 0f
        msr             DAIFClr,  #DAIF_F
0:
        ret

        .end

