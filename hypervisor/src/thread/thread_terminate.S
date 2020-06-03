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

        dmb             sy
        str             xzr, [x0, #(THREAD_SETUP_ENTRY * 8)]
        dsb             ish

        b               thread_standby

no_array:
        svc             #0

        .end

