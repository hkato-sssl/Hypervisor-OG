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
        .extern         thread_standby
        .extern         excvec_hyp
        .extern         exc_stack_top
        .extern         aarch64_cpu_no

        .section        ".text.hypervisor_start", "ax", %progbits
        .type           hypervisor_start, %function
        .balign         4
hypervisor_start:
        /* disable interrupts */

        msr             DAIFSet, #(DAIF_A | DAIF_I | DAIF_F)

        /* initialize processor status */

        adr             x0, excvec_hyp
        dsb             sy
        isb
        msr             VBAR_EL2, x0
        isb

        bl              exc_stack_top
        msr             SPSel, #1
        mov             sp, x0

        /* call an initial function */

        bl              aarch64_cpu_no
        cbnz            x0, thread_standby      /* secondary processor */

        bl              thread_stack_top
        msr             SPSel, #0
        mov             sp, x0
        bl              hypervisor              /* primary processor */

        b               thread_standby

        .end

