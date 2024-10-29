// SPDX-License-Identifier: Apache-2.0
/*
 * boot/init_processor_el1.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "config/system.h"
#include "lib/asm.h"
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/system_register/hcr_el2.h"

#define INITIAL_SPSR    (PSTATE_D | PSTATE_A | PSTATE_I | PSTATE_F | SPSR_M_EL1h)

        .global         init_processor_el1

        .extern         initial_exception_vector

        .section        ".text.startup.init_processor_el1", "ax", %progbits
        .type           init_processor_el1, %function
        .balign         4

init_processor_el1:
        msr             DAIFset, #DAIF_ALL
        mov             x28, lr             /* save LR */

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

        // check the current exception level

        mrs             x0, CurrentEL

        /* if the processor is in EL0 or EL3, loop forever */
        cbz             w0, .
        cmp             w0, #CURRENT_EL3
        beq             .

        cmp             w0, #CURRENT_EL1
        beq             el1

        // switch to EL1 from EL2

        mrs             x0, SCTLR_EL2
        mov             x1, #(SCTLR_I | SCTLR_C)
        bic             x0, x0, x1
        dsb             sy
        isb
        msr             SCTLR_EL2, x0
        isb

        bl              aarch64_icache_invd_all
        bl              aarch64_dcache_invd_all

        mov             x1, #INITIAL_SPSR
        adr             x2, el1
        msr             SPSR_EL2, x1
        msr             ELR_EL2, x2

        mov             x0, #HCR_EL2_RW
        msr             HCR_EL2, x0
        msr             HSTR_EL2, xzr
        msr             HACR_EL2, xzr
        msr             CPTR_EL2, xzr
        msr             CNTVOFF_EL2, xzr
        mrs             x0, MIDR_EL1
        msr             VPIDR_EL2, x0
        mrs             x0, MPIDR_EL1
        msr             VMPIDR_EL2, x0

        eret
el1:
        adr             x0, initial_exception_vector
        dsb             sy
        isb
        msr             VBAR_EL1, x0
        isb

        msr             SPSel, #1
        mov             x0, xzr
        mov             sp, x0

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

