/*
 * thread/thread_terminate.S
 *
 * (C) 2020 Hidekazu Kato
 */

#include "lib/asm.h"
#include "driver/aarch64/system_register.h"

        .global         thread_terminate

        .extern         thread_terminate

        .section        ".text.thread_terminate", "ax", %progbits
        .type           thread_terminate, %function
        .balign         4
thread_terminate:
        msr             DAIFSet, #(DAIF_A | DAIF_I | DAIF_F)
        msr             SPSel, #1
        msr             TPIDR_EL2, xzr

        adr             x0, thread_request_array
        mrs             ip0, MPIDR_EL1
        and             ip0, ip0, #0xff
        add             x0, x0, ip0, lsl #4

        dsb             sy
        mov             ip0, #-1            /* ALL F */
        str             ip0, [x0, #8]
        dmb             ish
        str             ip0, [x0]
        dmb             ish

watch_loop:
        ldr             ip0, [x0]
        cbnz            ip0, call_handler
        wfe
        b               watch_loop

call_handler:
        ldr             x0, [x0, #8]
        blr             ip0
        b               .

        .end

