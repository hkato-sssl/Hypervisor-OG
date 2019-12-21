/*
 * thread/thread_launch.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"
#include "driver/aarch64/system_register.h"
#include "hypervisor/thread.h"

        .global         thread_launch

        .extern         kernel_stack_top

        .section        ".text.thread_launch", "ax", %progbits
        .type           thread_launch, %function
        .balign         4
        /*
         * errno_t thread_launch(void *entry, void *sp, void *arg)
         */
thread_launch:
        msr             DAIFSet, #(DAIF_I | DAIF_F)

        /*
         * initialize SP, and allocate TLS
         */

        msr             SPSel, #0
        sub             sp, x1, #TLS_SIZE
        mov             x1, sp
        msr             TPIDR_EL2, x1

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
        blr             x1

        svc             #0

        ret

        .end

