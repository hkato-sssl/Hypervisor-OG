// SPDX-License-Identifier: Apache-2.0
/*
 * boot/start.S - the startup routine for the primary processor
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include "lib/asm.h"
#include "driver/aarch64/system_register.h"

        .extern         __bss_start
        .extern         __bss_end

        .extern         init_processor_el1
        .extern         exc_stack_top
        .extern         main_stack_top
        .extern         main

        .global         _start
        .section        ".text.entry", "ax", %progbits
        .type           _start, %function
_start:
        // initialize a processor and cache memory

        bl              init_processor_el1

        // initialize SP

        bl              exc_stack_top
        msr		        SPSel, #1
        mov             sp, x0

        bl              main_stack_top
        msr             SPSel, #0
        mov             sp, x0

        mrs             x0, MPIDR_EL1
        and             x0, x0, #0xff
        cbnz            x0, call_main

        // check parameters of .bss region

        adrl            x0, __bss_start
        tst             x0, #63
        bne             .                       // error loop
        adrl            x1, __bss_end
        tst             x1, #63
        bne             .                       // error loop
        subs            x1, x1, x0
        beq             .                       // error loop

        // clear .bss region
0:
        stp             xzr, xzr, [x0], #16
        stp             xzr, xzr, [x0], #16
        stp             xzr, xzr, [x0], #16
        stp             xzr, xzr, [x0], #16
        subs            x1, x1, #64
        bne             0b

        // call main()
call_main:
        mov             x0, xzr                 // argc = 0
        mov             x1, xzr                 // argv = NULL
        bl              main

        b               .                       // loop forever

        .end

