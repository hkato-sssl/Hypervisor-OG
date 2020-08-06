/*
 * boot/init_processor_el2.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include "lib/asm.h"
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/system_register/hcr_el2.h"

        .global         init_processor_el2

        .extern         initial_exception_vector

        .section        ".text.startup.init_processor_el2", "ax", %progbits
        .type           init_processor_el2, %function
        .balign         4

init_processor_el2:
        mov             x28, lr         // save LR

        msr             DAIFset, #DAIF_ALL
        adr             x0, initial_exception_vector
        msr             VBAR_EL2, x0
        isb

        msr             SPSel, #1
        mov             x0, xzr
        mov             sp, x0

        // initialize HCR_EL2

        dsb             sy
        isb
        mrs             x0, HCR_EL2
        orr             x1, x0, #(HCR_EL2_AMO | HCR_EL2_IMO | HCR_EL2_FMO)
        msr             HCR_EL2, x1
        isb

        // initialize SCTLR_EL2

        mov32           x0, (SCTLR_EE | SCTLR_WXN | SCTLR_I | SCTLR_C | SCTLR_M)
        mov             x1, #(SCTLR_SA | SCTLR_A)

        mrs             x2, SCTLR_EL2
        bic             x3, x2, x0
        orr             x3, x3, x1
        dsb             sy
        isb
        msr             SCTLR_EL2, x3
        isb

        // initialize and enable I-$

        bl              aarch64_icache_invd_all

        mrs             x0, SCTLR_EL2
        orr             x1, x0, #SCTLR_I
        dsb             sy
        isb
        msr             SCTLR_EL2, x1
        isb

        // initialize  D-$

        bl              aarch64_dcache_invd_all

        // initialize ACTLR

        dsb             sy
        isb
        msr             ACTLR_EL2, xzr
        isb

        // Traps Non-secure accesses to SIMD and floating-point
        // functionality to EL2, from both Execution states.

        mrs             x0, CPTR_EL2
        orr             x1, x0, #CPTR_TFP
        msr             CPTR_EL2, x1
        isb

        br              x28

        .end

