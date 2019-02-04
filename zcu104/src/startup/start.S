/*
 * startup/start.S - the startup routine for the primary processor
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         _start
        .section        ".text.start", "ax", %progbits
        .type           _start, %function
_start:
	b		.

        .end

