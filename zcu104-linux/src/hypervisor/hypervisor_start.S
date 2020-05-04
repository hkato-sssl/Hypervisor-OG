/*
 * hypervisor_start.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"
#include "driver/aarch64/system_register.h"

        .global         hypervisor_start

        .extern         hypervisor
        .extern         thread_stack_top
        .extern         thread_start
        .extern         excvec_hyp

        .section        ".text.hypervisor_start", "ax", %progbits
        .type           hypervisor_start, %function
        .balign         4
hypervisor_start:
        /* disable interrupts */

        msr             DAIFSet, #(DAIF_A | DAIF_I | DAIF_F)

        /* initialize an exception vector */

        adr             x0, excvec_hyp
        dsb             sy
        isb
        msr             VBAR_EL2, x0
        isb

        /* call the entry function */

        bl              thread_stack_top
        mov             x1, x0
        mov             x2, xzr
        adr             x0, hypervisor
        b               thread_start

        .end

