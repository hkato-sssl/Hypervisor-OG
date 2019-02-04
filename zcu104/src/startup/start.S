/*
 * startup/start.S - the startup routine for the primary processor
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include "lib/aarch64.h"

	.extern		init_exception_vector
	.extern		main_stack_region

        .global         _start
        .section        ".text.start", "ax", %progbits
        .type           _start, %function
_start:
	// set initial status

	msr		DAIFset, #DAIF_ALL
	msr		SPsel, #1
	adr		x1, init_exception_vector
	msr		VBAR_EL2, x1
	adr		x1, (main_stack_region + STACK_SIZE)
	mov		sp, x1

        .end

