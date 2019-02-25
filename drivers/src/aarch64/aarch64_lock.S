/*
 * aarch64/aarch64_lock.S
 *
 * (C) 2019 Hidekazu Kato
 */

/* CAUTION:
 *
 * ここでの実装はDAIF.Iのみを操作対象としている為、Non-secure mode専用
 * APIとなる。
 * Secure modeで動作させる場合は、実装そのものを変更する必要がある。
 */

#include "lib/asm.h"
#include "driver/aarch64/system_register.h"

        .global         aarch64_lock
        .global         aarch64_unlock

        .section        ".text.aarch64_lock", "ax", %progbits
        .type           aarch64_lock, %function
        .balign         4
aarch64_lock:
        mrs             x0,  DAIF
        tbnz            x0,  #7, no_lock        /* test DAIF.I */

        /* mask interrupts */
        msr             DAIFSet, #DAIF_I
        mov             x0,  #1
        ret
no_lock:
        mov             x0,  #0
        ret

        .section        ".text.aarch64_unlock", "ax", %progbits
        .type           aarch64_unlock, %function
        .balign         4
aarch64_unlock:
        cbz             x0,  no_unlock
        msr             DAIFClr, #DAIF_I
no_unlock:
        ret

        .end

