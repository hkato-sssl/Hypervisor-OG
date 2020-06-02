/*
 * thread/thread_terminate.S
 *
 * (C) 2020 Hidekazu Kato
 */

#include "lib/asm.h"
#include "driver/aarch64/system_register.h"
#include "thread_local.h"

        .global         thread_terminate

        .extern         aarch64_cpu_no
        .extern         thread_start
        .extern         thread_setup_array

        .section        ".text.thread_terminate.S", "ax", %progbits
        .type           thread_terminate.S, %function
        .balign         4
thread_terminate:
        msr             DAIFSet, #(DAIF_A | DAIF_I | DAIF_F)

        bl              aarch64_cpu_no
        bl              thread_setup_array
        cbz             x0, no_array

        mov             ip0, x0
        dmb             ish
        str             xzr, [ip0]
        dsb             ish
loop:
        wfe
        ldr             x0,  [ip0]
        cbz             x0,  loop

        ldr             x1,  [ip0, #(THREAD_SETUP_SP * 8)]
        ldr             x2,  [ip0, #(THREAD_SETUP_ARG0 * 8)]
        bl              thread_start
        svc             #0          

no_array:
        svc             #0

        .end

