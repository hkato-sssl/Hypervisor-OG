/*
 * thread/thread_start.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"
#include "lib/bit.h"
#include "driver/aarch64/system_register.h"
#include "hypervisor/thread.h"

        .global         thread_start

        .extern         exc_stack_top
        .extern         thread_stack_top
        .extern         thread_terminate

        .section        ".text.thread_start", "ax", %progbits
        .type           thread_start, %function
        .balign         4
        /*
         * volatile void thread_start(void *entry, void *arg)
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

        mov             x20, x0
        mov             x21, x1
        bl              thread_stack_top

        sub             x0, x0, #TLS_SIZE
        msr             TPIDR_EL2, x0
        msr             SPSel, #0
        mov             sp, x0

        /*
         * initialize TLS
         */

        stp             x20, x21, [sp, #-16]!

        mrs             x0, TPIDR_EL2
        mov             x1, #0
        mov             x2, #TLS_SIZE
        bl              memset

        bl              exc_stack_top
        mrs             x1, TPIDR_EL2
        str             x0, [x1, #(TLS_EXCEPTION_SP * 8)]
        ldp             x1, x0,  [sp], #16

        msr             DAIFClr, DAIF_A
        blr             x1

        b               thread_terminate

        .end

