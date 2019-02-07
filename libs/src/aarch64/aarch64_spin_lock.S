/*
 * aarch64/aarch64_spin_lock.S
 *
 * (C) 2019 Hidekazu Kato
 */

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
        dmb             ish
        mov             x0,  xzr
        ret

        .section        ".text.aarch64_spin_trylock", "ax", %progbits
        .type           aarch64_spin_trylock, %function
        .balign         4
aarch64_spin_trylock:
        mrs             x1,  MPIDR_EL1
        dsb             sy
        ldxr            w2,  [x0]
        cbnz            w2,  already_locked
        stxr            w2,  w1,  [x0]
        sub             x0,  xzr, x2
        ret

already_locked:
        mov             x0,  #-EBUSY
        ret

        .section        ".text.aarch64_spin_lock", "ax", %progbits
        .type           aarch64_spin_lock, %function
        .balign         4
        stp             x19, lr,  [sp,  #-16]!
        mov             x19, x0

lock_loop:
        bl              aarch64_spin_trylock
        cbz             x0, lock_exit

        /* delay */
        yield
        yield
        yield
        yield

        /* try again */
        mov             x0,  x19
        b               lock_loop

lock_exit:
        ldp             x19, lr,  [sp],  #16
        ret
        

aarch64_spin_lock:
        
        .section        ".text.aarch64_spin_unlock", "ax", %progbits
        .type           aarch64_spin_lock, %function
        .balign         4
aarch64_spin_unlock:
        dsb             sy
        str             wzr, [x0]
        dsb             ish
        ret

        .end

