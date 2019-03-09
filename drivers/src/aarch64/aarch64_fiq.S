/*
 * aarch64/aarch64_fiq.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"
#include "driver/aarch64/system_register.h"

        .global         aarch64_lock_fiq
        .global         aarch64_lock_fiq

        .section        ".text.aarch64_lock_fiq", "ax", %progbits
        .type           aarch64_lock_fiq, %function
        .balign         4
aarch64_lock_fiq:
        mrs             x0,  DAIF
        tbnz            x0,  #6, 0f             /* test DAIF.F */

        /* mask interrupts */
        msr             DAIFSet, #DAIF_F
        mov             x0,  #1
        ret

        /* FIQ is already locked */
0:
        mov             x0,  #0
        ret

        .section        ".text.aarch64_unlock_fiq", "ax", %progbits
        .type           aarch64_unlock_fiq, %function
        .balign         4
aarch64_unlock_fiq:
        cbz             x0, 0f
        msr             DAIFClr, #DAIF_F
0:
        ret

        .end

