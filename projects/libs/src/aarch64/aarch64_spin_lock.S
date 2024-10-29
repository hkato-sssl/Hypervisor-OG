// SPDX-License-Identifier: Apache-2.0
/*
 * aarch64/aarch64_spin_lock.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"
#include "lib/system/errno.h"

        .global         aarch64_spin_lock_init
        .global         aarch64_spin_trylock
        .global         aarch64_spin_lock
        .global         aarch64_spin_unlock

        .section        ".text.aarch64_spin_lock_init", "ax", %progbits
        .type           aarch64_spin_lock_init, %function
        .balign         4
aarch64_spin_lock_init:
        str             wzr, [x0]
        dsb             ish
        mov             x0,  xzr
        ret

        /*
         * ldxr命令前にdsb命令を実行する必要性は無いが、lock/unlock間で
         * メモリアクセスを明示的にシリアライズさせる事でシステム全体の
         * メモリアクセス手順を明確にする意図によりdsb命令を実行させる。
         */
        .section        ".text.aarch64_spin_trylock", "ax", %progbits
        .type           aarch64_spin_trylock, %function
        .balign         4
aarch64_spin_trylock:
        mov             x2,  x0
        mrs             x1,  MPIDR_EL1
        dsb             sy
        ldxr            w0,  [x2]
        cbnz            w0,  already_locked
        stxr            w0,  w1,  [x2]
        cbz             x0,  trylock_exit
        mov             x0,  #-EAGAIN
trylock_exit:
        ret

already_locked:
        mov             x0,  #-EBUSY
        ret

        .section        ".text.aarch64_spin_lock", "ax", %progbits
        .type           aarch64_spin_lock, %function
        .balign         4
aarch64_spin_lock:
        stp             x19, lr,  [sp,  #-16]!
        mov             x19, x0

lock_loop:
        bl              aarch64_spin_trylock
        cbz             x0, lock_exit

        /* wait and try again */
        wfe
        mov             x0,  x19
        b               lock_loop

lock_exit:
        ldp             x19, lr,  [sp],  #16
        ret
        
        .section        ".text.aarch64_spin_unlock", "ax", %progbits
        .type           aarch64_spin_lock, %function
        .balign         4
aarch64_spin_unlock:
        dsb             sy
        str             wzr, [x0]
        dsb             ish
        ret

        .end

