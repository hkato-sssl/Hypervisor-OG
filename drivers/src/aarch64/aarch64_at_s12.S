/*
 * aatch64/aarch64_at_s12.S
 *
 * (C) 2020 Hidekazu Kato
 */
        .global         aarch64_at_s12e0r
        .global         aarch64_at_s12e0w
        .global         aarch64_at_s12e1r
        .global         aarch64_at_s12e1w

        .section        ".text.aarch64_at_s12e0r", "ax", %progbits
        .type           aarch64_at_s12e0r, %function
        .balign         4
aarch64_at_s12e0r:
        dsb             osh
        at              s12e0r, x0
        dsb             nsh
        isb
        mrs             x0, PAR_EL1
        ret

        .section        ".text.aarch64_at_s12e0w", "ax", %progbits
        .type           aarch64_at_s12e0w, %function
        .balign         4
aarch64_at_s12e0w:
        dsb             osh
        at              s12e0w, x0
        dsb             nsh
        isb
        mrs             x0, PAR_EL1
        ret

        .section        ".text.aarch64_at_s12e1r", "ax", %progbits
        .type           aarch64_at_s12e1r, %function
        .balign         4
aarch64_at_s12e1r:
        dsb             osh
        at              s12e1r, x0
        dsb             nsh
        isb
        mrs             x0, PAR_EL1
        ret

        .section        ".text.aarch64_at_s12e1w", "ax", %progbits
        .type           aarch64_at_s12e1w, %function
        .balign         4
aarch64_at_s12e1w:
        dsb             osh
        at              s12e1w, x0
        dsb             nsh
        isb
        mrs             x0, PAR_EL1
        ret

        .end

