/*
 * aarch64/cache/aarch64_poc_line_sz.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         aarch64_poc_line_sz

        .section        ".text.aarch64_poc_line_sz", "ax", %progbits
        .type           aarch64_poc_line_sz, %function
        .balign         4
aarch64_poc_line_sz:
        mrs             x0,  CLIDR_EL1
        lsr             x1,  x0,  #(24 - 1)
        ands            x1,  x1,  #0x0e
        beq             return_zero
        msr             CSSELR_EL1, x1
        isb
        mrs             x2,  CCSIDR_EL1
        and             x2,  x2,  #7
        add             x2,  x2,  #4
        mov             x0,  #1
        lsl             x0,  x0,  x2
        ret

return_zero:
        mov             x0,  xzr
        ret

        .end

