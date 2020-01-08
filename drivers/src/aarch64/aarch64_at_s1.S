/*
 * aatch64/aarch64_at_s1.S
 *
 * (C) 2020 Hidekazu Kato
 */
        .global         aarch64_at_s1e0r
        .global         aarch64_at_s1e0w
        .global         aarch64_at_s1e1r
        .global         aarch64_at_s1e1w

        .section        ".text.aarch64_at_s1e0r", "ax", %progbits
        .type           aarch64_at_s1e0r, %function
        .balign         4
aarch64_at_s1e0r:
        at              s1e0r, x0
        dsb             nsh
        isb
        mrs             x0, PAR_EL1
        ret

        .section        ".text.aarch64_at_s1e0w", "ax", %progbits
        .type           aarch64_at_s1e0w, %function
        .balign         4
aarch64_at_s1e0w:
        at              s1e0w, x0
        dsb             nsh
        isb
        mrs             x0, PAR_EL1
        ret

        .section        ".text.aarch64_at_s1e1r", "ax", %progbits
        .type           aarch64_at_s1e1r, %function
        .balign         4
aarch64_at_s1e1r:
        at              s1e1r, x0
        dsb             nsh
        isb
        mrs             x0, PAR_EL1
        ret

        .section        ".text.aarch64_at_s1e1w", "ax", %progbits
        .type           aarch64_at_s1e1w, %function
        .balign         4
aarch64_at_s1e1w:
        at              s1e1w, x0
        dsb             nsh
        isb
        mrs             x0, PAR_EL1
        ret

        .end

