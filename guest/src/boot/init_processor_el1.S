/*
 * boot/init_processor_el1.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include "lib/asm.h"
#include "driver/aarch64/system_register.h"

        .global         init_processor_el1

        .extern         initial_exception_vector

        .section        ".text.startup.init_processor_el1", "ax", %progbits
        .type           init_processor_el1, %function
        .balign         4

init_processor_el1:
        mov             x28, lr         // save LR

        msr             DAIFset, #DAIF_ALL
        adr             x0, initial_exception_vector
        dsb             sy
        isb
        msr             VBAR_EL1, x0
        isb

        msr             SPSel, #1
        mov             x0, xzr
        mov             sp, x0

        // initialize SCTLR_EL1

        mov32           x0, (SCTLR_EE | SCTLR_E0E | SCTLR_WXN | SCTLR_I | SCTLR_C | SCTLR_M)
        mov             x1, #(SCTLR_SA | SCTLR_A)

        mrs             x2, SCTLR_EL1
        bic             x3, x2, x0
        orr             x3, x3, x1
        dsb             sy
        isb
        msr             SCTLR_EL1, x3
        isb

        // initialize and enable I-$

        bl              aarch64_icache_invd_all

        mrs             x0, SCTLR_EL1
        orr             x1, x0, #SCTLR_I
        dsb             sy
        isb
        msr             SCTLR_EL1, x1
        isb

        // initialize  D-$

        bl              aarch64_dcache_invd_all

        // initialize ACTLR

        dsb             sy
        isb
        msr             ACTLR_EL1, xzr
        isb

        // Trap any instruction in EL0 or EL1 that uses registers
        // associated with Advanced SIMD and Floating-point execution.

        msr             CPACR_EL1, xzr
        isb

        br              x28

        .end

