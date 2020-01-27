/*
 * aatch64/at/aarch64_at_s1e2.S
 *
 * (C) 2020 Hidekazu Kato
 */
        .global         aarch64_at_s1e2r
        .global         aarch64_at_s1e2w

        .section        ".text.aarch64_at_s1e2r", "ax", %progbits
        .type           aarch64_at_s1e2r, %function
        .balign         4
aarch64_at_s1e2r:
        at              s1e2r, x0
        dsb             nsh
        isb
        mrs             x0, PAR_EL1
        ret

        .section        ".text.aarch64_at_s1e2w", "ax", %progbits
        .type           aarch64_at_s1e2w, %function
        .balign         4
aarch64_at_s1e2w:
        at              s1e2w, x0
        dsb             nsh
        isb
        mrs             x0, PAR_EL1
        ret

        .end

