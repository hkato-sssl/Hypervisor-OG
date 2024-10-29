// SPDX-License-Identifier: Apache-2.0
/*
 * vpc/vpc_ctx_fpu.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"
#include "lib/bit.h"
#include "hypervisor/vpc_register.h"

        .global         vpc_load_ctx_fpu

        .section        ".text.vpc_load_ctx_fpu", "ax", %progbits
        .type           vpc_load_ctx_fpu, %function
        .balign         4
        /*
         * void vpc_load_ctx_fpu(uint64_t *regs);
         */
vpc_load_ctx_fpu:
        mrs             x1, CPTR_EL2
        bic             x2, x1, BIT(10)
        msr             CPTR_EL2, x2
        isb

        ldr             x2, [x0, #(VPC_FPCR * 8)]
        ldr             x3, [x0, #(VPC_FPSR * 8)]
        ldr             x4, [x0, #(VPC_FPEXC32_EL2 * 8)]

        add             x0, x0, #(VPC_Q0 * 8)
        ldp             q0, q1, [x0], #32
        ldp             q2, q3, [x0], #32
        ldp             q4, q5, [x0], #32
        ldp             q6, q7, [x0], #32
        ldp             q8, q9, [x0], #32
        ldp             q10, q11, [x0], #32
        ldp             q12, q13, [x0], #32
        ldp             q14, q15, [x0], #32
        ldp             q16, q17, [x0], #32
        ldp             q18, q19, [x0], #32
        ldp             q20, q21, [x0], #32
        ldp             q22, q23, [x0], #32
        ldp             q24, q25, [x0], #32
        ldp             q26, q27, [x0], #32
        ldp             q28, q29, [x0], #32
        ldp             q30, q31, [x0]

        dsb             ish
        msr             FPEXC32_EL2, x4
        msr             FPSR, x3
        msr             FPCR, x2
        isb

        msr             CPTR_EL2, x1
        isb
        ret

        .section        ".text.vpc_store_ctx_fpu", "ax", %progbits
        .type           vpc_store_ctx_fpu, %function
        .balign         4
        /*
         * void vpc_store_ctx_fpu(uint64_t *regs);
         */
vpc_store_ctx_fpu:
        mrs             x1, CPTR_EL2
        bic             x2, x1, BIT(10)
        msr             CPTR_EL2, x2
        isb

        mov             x2, #(VPC_FPCR * 8)
        add             x0, x0, x2

        mrs             x2, FPCR
        mrs             x3, FPSR
        mrs             x4, FPEXC32_EL2
        str             x2, [x0, #(VPC_FPCR * 8)]
        str             x3, [x0, #(VPC_FPSR * 8)]
        str             x4, [x0, #(VPC_FPEXC32_EL2 * 8)]

        add             x0, x0, #(VPC_Q0 * 8)
        stp             q0, q1, [x0], #32
        stp             q2, q3, [x0], #32
        stp             q4, q5, [x0], #32
        stp             q6, q7, [x0], #32
        stp             q8, q9, [x0], #32
        stp             q10, q11, [x0], #32
        stp             q12, q13, [x0], #32
        stp             q14, q15, [x0], #32
        stp             q16, q17, [x0], #32
        stp             q18, q19, [x0], #32
        stp             q20, q21, [x0], #32
        stp             q22, q23, [x0], #32
        stp             q24, q25, [x0], #32
        stp             q26, q27, [x0], #32
        stp             q28, q29, [x0], #32
        stp             q30, q31, [x0]

        dsb             ish
        msr             CPTR_EL2, x1
        isb
        ret

        .end

