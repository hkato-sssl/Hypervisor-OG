/*
 * system/cpu_lock_interrupts.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         cpu_lock_interrupts
        .extern         aarch64_lock_interrupts

        .section        ".text.cpu_lock_interrupts", "ax", %progbits
        .type           cpu_lock_interrupts, %function
        .balign         4
cpu_lock_interrupts:
        b               aarch64_lock_interrupts

        .end

