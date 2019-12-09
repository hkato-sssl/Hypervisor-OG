/*
 * test/vpc/launch_asm.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         test_vpc_launch_entry

        .section        ".text.test_vpc_launch_entry", "ax", %progbits
        .type           test_vpc_launch_entry, %function
        .balign         4
test_vpc_launch_entry:
	b		.
	mrs		x0, CurrentEL
	b		.

        .end

