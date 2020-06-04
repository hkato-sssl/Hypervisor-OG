/*
 * thread/thread_standby.S
 *
 * (C) 2020 Hidekazu Kato
 */

#include "lib/asm.h"
#include "driver/aarch64/system_register.h"
#include "hypervisor/thread.h"
#include "thread_local.h"

        .global         thread_standby

        .extern         aarch64_cpu_no
        .extern         thread_stack_top
        .extern         thread_parameter_array
        .extern         thread_start

        .section        ".text.thread_standby", "ax", %progbits
        .type           thread_standby, %function
        .balign         4
thread_standby:
        msr             DAIFSet, #(DAIF_A | DAIF_I | DAIF_F)

        bl              thread_stack_top
        msr             SPSel, #0
        mov             sp, x0

        bl              aarch64_cpu_no
        bl              thread_parameter_array
        cbz             x0, no_array
        sevl

standby:
        wfe
        ldr             ip0, [x0, #(THREAD_PARAMETER_ENTRY * 8)]
        cbz             ip0, standby

launch:
        b               thread_start

no_array:
        svc             #0

        .end

