/*
 * aarch64/aarch64_lock.S
 *
 * (C) 2019 Hidekazu Kato
 */

/* CAUTION:
 *
 * ここで実装されるAPIはNon-secure mode専用となる。
 * Secure modeで動作させる場合は、PSTATE_MASK/DAIF_BITSの定義を修正
 * する必要がある。
 */

#include "driver/aarch64/system_register.h"

#define PSTATE_MASK     PSTATE_I
#define DAIF_BITS       DAIF_I

        .global         aarch64_lock
        .global         aarch64_unlock

        .section        ".text.aarch64_lock", "ax", %progbits
        .type           aarch64_lock, %function
        .balign         4
aarch64_lock:
        mrs             x0,  DAIF
        ands            x0,  x0,  #PSTATE_MASK
        beq             lock_exit

        /* disable interrupts */
        msr             DAIFSet, #DAIF_BITS
        mov             x0,  #1
lock_exit:
        ret

        .section        ".text.aarch64_unlock", "ax", %progbits
        .type           aarch64_unlock, %function
        .balign         4
aarch64_unlock:
        cbz             x0,  exit
        msr             DAIFClr, #DAIF_BITS
exit:
        ret

        .end

