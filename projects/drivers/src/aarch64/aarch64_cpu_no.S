// SPDX-License-Identifier: Apache-2.0
/*
 * aarch64/aarch64_cpu_no.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         aarch64_cpu_no

        .section        ".text.aarch64_cpu_no", "ax", %progbits
        .type           aarch64_cpu_no, %function
        .balign         4
aarch64_cpu_no:
        mrs             x1,  MPIDR_EL1
        uxtb            w0,  w1
        ret

        .end

