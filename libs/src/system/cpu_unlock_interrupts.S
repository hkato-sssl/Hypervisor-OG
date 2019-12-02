/*
 * system/cpu_unlock_interrupts.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         cpu_unlock_interrupts
        .extern         aarch64_unlock_interrupts

        .section        ".text.cpu_unlock_interrupts", "ax", %progbits
        .type           cpu_unlock_interrupts, %function
        .balign         4
cpu_unlock_interrupts:
        b               aarch64_unlock_interrupts

        .end

