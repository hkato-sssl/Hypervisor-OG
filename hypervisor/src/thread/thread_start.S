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

        .section        ".text.thread_start", "ax", %progbits
        .type           thread_start, %function
        .balign         4
        /*
         * volatile void thread_start(void *entry, void *sp, void *arg)
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

        msr             SPSel, #0
        sub             x1, x1, #TLS_SIZE
        bic             x1, x1, #BITS(3, 0)
        msr             TPIDR_EL2, x1
        mov             sp, x1

        /*
         * initialize TLS
         */

        stp             x0, x2, [sp, #-16]!

        mrs             x0, TPIDR_EL2
        mov             x1, #0
        mov             x2, #TLS_SIZE
        bl              memset

        bl              exc_stack_top
        mrs             x1, TPIDR_EL2
        str             x0, [x1, #(TLS_EXCEPTION_SP * 8)]
        ldp             x1, x0, [sp], #16

        msr             DAIFClr, DAIF_A
        blr             x1

        svc             #0

        ret

        .end

