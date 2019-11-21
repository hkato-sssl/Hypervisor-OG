/*
 * system/assert_exception.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         assert_exception

        .section        ".text.assert_exception", "ax", %progbits
        .type           assert_exception, %function
        .balign         4
assert_exception:
        smc             #0xB001
        b               assert_exception

        .end

