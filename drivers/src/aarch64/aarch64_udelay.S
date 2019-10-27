/*
 * aarch64/aarch64_udelay.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"

        .global         aarch64_udelay

        .section        ".text.aarch64_udelay", "ax", %progbits
        .type           aarch64_udelay, %function
        .balign         4

	/*
         * void aarch64_udelay(uint32_t usec)
         */
aarch64_udelay:
	isb
	mrs		x1, CNTPCT_EL0
	mrs		x2, CNTFRQ_EL0		// CNTFRQ_EL0 is 32-bit register
	cbz		w0, exit

	mov32		w3, 1000000
	udiv		w2, w2, w3
	mov		x3, #1
	umaddl		x2, w2, w0, x3
loop:
	isb
	mrs		x3, CNTPCT_EL0
	sub		x0, x3, x1
	cmp		x0, x2
	b.cs		exit
	yield
	b		loop
exit:
	ret

        .end

