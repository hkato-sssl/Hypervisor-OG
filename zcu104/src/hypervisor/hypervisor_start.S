/*
 * hypervisor_start.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"
#include "driver/aarch64/system_register.h"

        .global         hypervisor_start

        .extern         hypervisor
        .extern         thread_launch
        .extern         excvec_hyp

        .section        ".text.hypervisor_start", "ax", %progbits
        .type           hypervisor_start, %function
        .balign         4
hypervisor_start:
        /* disable interrupts */

        msr             DAIFSet, #(DAIF_I | DAIF_F)

        /* reset SP */

        bl              kernel_stack_top
        msr             SPSel, #1
        mov             sp, x0

        /* initialize an exception vector */

        adr             x0, excvec_hyp
        msr             VBAR_EL2, x0
        isb

        /* launch a thread */

        bl              main_stack_top
        mov             x1, x0
        adr             x0, hypervisor
        mov             x2, xzr
        bl              thread_launch

        svc             #0

        .end

