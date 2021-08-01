/*
 * system/spin_lock.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         spin_lock_init
        .global         spin_lock
        .global         spin_unlock

        .extern         aarch64_spin_lock_init
        .extern         aarch64_spin_lock
        .extern         aarch64_spin_unlock

        .section        ".text.spin_lock_init", "ax", %progbits
        .type           spin_lock_init, %function
        .balign         4
spin_lock_init:
        b               aarch64_spin_lock_init

        .section        ".text.spin_lock", "ax", %progbits
        .type           spin_lock, %function
        .balign         4
spin_lock:
        b               aarch64_spin_lock

        .section        ".text.spin_unlock", "ax", %progbits
        .type           spin_unlock, %function
        .balign         4
spin_unlock:
        b               aarch64_spin_unlock

        .end

