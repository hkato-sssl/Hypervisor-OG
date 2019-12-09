/*
 * config/memory_asm.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include "lib/asm.h"

        .global         main_stack_top
        .global         exc_stack_top

        .extern         main_stack_region
        .extern         exc_stack_region

        .section        ".text.main_stack_top", "ax", %progbits
        .type           main_stack_top, %function
        .balign         4
        /*
         * void *main_stack_top(uint32_t cpu_no);
         */
main_stack_top:
        cmp             w0, #CONFIG_NR_CPUS
        bge             999f

        add             x0, x0, #1
        mov             w1, #CONFIG_STACK_SIZE
        adrl            x2, main_stack_region
        umaddl          x0, w0, w1, x2
        ret
999:
        mov             x0, xzr
        ret

        .section        ".text.exc_stack_top", "ax", %progbits
        .type           exc_stack_top, %function
        .balign         4
        /*
         * void *exc_stack_top(uint32_t cpu_no);
         */
exc_stack_top:
        cmp             w0, #CONFIG_NR_CPUS
        bge             999f

        add             x0, x0, #1
        mov             w1, #CONFIG_STACK_SIZE
        adrl            x2, exc_stack_region
        umaddl          x0, w0, w1, x2
        ret
999:
        mov             x0, xzr
        ret

        .end

