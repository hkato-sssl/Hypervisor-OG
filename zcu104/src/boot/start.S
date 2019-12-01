/*
 * boot/start.S - the startup routine for the primary processor
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include "lib/asm.h"
#include "driver/aarch64/system_register.h"

        .extern         main_stack_region
        .extern         __bss_start
        .extern         __bss_end

        .extern         init_processor_el2
        .extern         main

        .global         _start
        .section        ".text.entry", "ax", %progbits
        .type           _start, %function
_start:
        // initialize a processor and cache memory

        bl              init_processor_el2

        // initialize SP

        msr		SPsel, #0
        mov		x1, xzr
        mov		sp, x1

        msr             SPsel, #1
        adrl            x1, (main_stack_region + CONFIG_STACK_SIZE)
        mov             sp, x1

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

        mov             x0, xzr                 // argc = 0
        mov             x1, xzr                 // argv = NULL
        bl              main

        b               .                       // loop forever

        .end

