/*
 * system/cpu_no.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         cpu_no
	.extern		aarch64_cpu_no

        .section        ".text.cpu_no", "ax", %progbits
        .type           cpu_no, %function
        .balign         4
cpu_no:
	b		aarch64_cpu_no

        .end

