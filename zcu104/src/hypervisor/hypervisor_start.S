/*
 * hypervisor_start.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         hypervisor_start

        .extern         hypervisor

        .section        ".text.hypervisor_start", "ax", %progbits
        .type           hypervisor_start, %function
        .balign         4
hypervisor_start:
        /* reset SP */

        bl              main_stack_top
        mov             sp, x0

        bl              hypervisor
        svc             #0

        .end

