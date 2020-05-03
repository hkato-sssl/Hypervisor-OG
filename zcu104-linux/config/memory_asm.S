/*
 * config/memory_asm.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include "lib/asm.h"

        .global         main_stack_top
        .global         exc_stack_top
        .global         kernel_stack_top

        .extern         main_stack_region
        .extern         exc_stack_region
        .extern         kernel_stack_region

        .section        ".text.main_stack_top", "ax", %progbits
        .type           main_stack_top, %function
        .balign         4
        /*
         * void *main_stack_top(void);
         */
main_stack_top:
        mrs             x0, MPIDR_EL1
        and             x0, x0, #0xff
        add             x0, x0, #1
        mov             w1, #CONFIG_STACK_SIZE
        adrl            x2, main_stack_region
        umaddl          x0, w0, w1, x2
        ret

        .section        ".text.exc_stack_top", "ax", %progbits
        .type           exc_stack_top, %function
        .balign         4
        /*
         * void *exc_stack_top(void);
         */
exc_stack_top:
        mrs             x0, MPIDR_EL1
        and             x0, x0, #0xff
        add             x0, x0, #1
        mov             w1, #CONFIG_EXC_STACK_SIZE
        adrl            x2, exc_stack_region
        umaddl          x0, w0, w1, x2
        ret

        .section        ".text.kernel_stack_top", "ax", %progbits
        .type           kernel_stack_top, %function
        .balign         4
        /*
         * void *emg_stack_top(void);
         */
kernel_stack_top:
        mrs             x0, MPIDR_EL1
        and             x0, x0, #0xff
        add             x0, x0, #1
        mov             w1, #CONFIG_KERNEL_STACK_SIZE
        adrl            x2, kernel_stack_region
        umaddl          x0, w0, w1, x2
        ret

        .end

