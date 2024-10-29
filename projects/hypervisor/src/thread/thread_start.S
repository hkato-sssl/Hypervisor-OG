// SPDX-License-Identifier: Apache-2.0
/*
 * thread/thread_start.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"
#include "lib/bit.h"
#include "driver/aarch64/system_register.h"
#include "hypervisor/thread.h"
#include "thread_local.h"

        .global         thread_start

        .extern         exc_stack_top
        .extern         aarch64_cpu_no
        .extern         thread_stack_top
        .extern         thread_parameter_array
        .extern         thread_terminate

        .section        ".text.thread_start", "ax", %progbits
        .type           thread_start, %function
        .balign         4
        /*
         * volatile void thread_start(void)
         */
thread_start:
        /*
         * initialize proessor status
         */

        msr             DAIFSet, #(DAIF_A | DAIF_I | DAIF_F)
        clrex

        /*
         * initialize SP, and allocate TLS
         */

        bl              thread_stack_top
        sub             x0, x0, #TLS_SIZE
        msr             TPIDR_EL2, x0
        msr             SPSel, #0
        mov             sp, x0

        /*
         * initialize TLS
         */

        mrs             x0, TPIDR_EL2
        mov             x1, #0
        mov             x2, #TLS_SIZE
        bl              memset

        bl              exc_stack_top
        mrs             x1, TPIDR_EL2
        str             x0, [x1, #(TLS_EXCEPTION_SP * 8)]

        /*
         * load the parameters
         */

        bl              aarch64_cpu_no
        bl              thread_parameter_array
        ldr             ip0, [x0, #(THREAD_PARAMETER_ENTRY* 8)]
        ldp             x2,  x3,  [x0, #(THREAD_PARAMETER_ARG(2) * 8)]
        ldp             x4,  x5,  [x0, #(THREAD_PARAMETER_ARG(4) * 8)]
        ldp             x6,  x7,  [x0, #(THREAD_PARAMETER_ARG(6) * 8)]
        ldp             x0,  x1,  [x0, #(THREAD_PARAMETER_ARG(0) * 8)]

        /*
         * call the entry routine
         */

        msr             DAIFClr, DAIF_A
        blr             ip0

        b               thread_terminate

        .end

