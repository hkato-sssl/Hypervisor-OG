/*
 * init/init_processor_el2.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include "lib/asm.h"
#include "driver/aarch64/system_register.h"

        .global         init_processor_el2

        .extern         initial_exception_vector

        .section        ".text.init_processor_el2", "ax", %progbits
        .type           init_processor_el2, %function
        .balign         4

init_processor_el2:
        dsb             sy              // ensure all memory accesses of previous instructions
        mov             x28, lr         // save LR

        msr             DAIFset, #DAIF_ALL
        adr             x0, initial_exception_vector
        msr             VBAR_EL2, x0
        isb

        msr             SPsel, #1
        mov             x0, xzr
        mov             sp, x0

        // initialize SCTLR_EL2

        mov             x0, #(SCTLR_EE | SCTLR_WXN)
        orr             x0, x0, #SCTLR_C
        orr             x0, x0, #SCTLR_M
        mov             x1, #(SCTLR_I | SCTLR_SA | SCTLR_A)

        mrs             x2, SCTLR_EL2
        bic             x3, x2, x0
        orr             x3, x3, x1
        msr             SCTLR_EL2, x3
        isb

        // invalidate I-$ and D-$

        bl              aarch64_icache_invd_all
        bl              aarch64_dcache_invd_all

        // initialize ACTLR

        msr             ACTLR_EL2, xzr
        isb

        br              x28

        .end

