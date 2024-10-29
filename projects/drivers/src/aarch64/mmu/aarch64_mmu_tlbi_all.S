// SPDX-License-Identifier: Apache-2.0
/*
 * driver/aarch64/mmu/aarch64_mmu_tlbi_all.S
 *
 * (C) 2019 Hidekazu Kato
 */

 #include "lib/asm.h"
 #include "lib/system/errno.h"

        .global         aarch64_mmu_tlbi_all

        .section        ".text.aarch64_mmu_tlbi_all", "ax", %progbits
        .type           aarch64_mmu_tlbi_all, %function
        .balign         4
aarch64_mmu_tlbi_all:
        BR_EL           table
table:
        .quad           op_el0
        .quad           op_el1
        .quad           op_el2
        .quad           op_el3

op_el0:
        mov             x0,  #-EPERM
        ret
op_el1:
        tlbi            vmalle1
        mov             x0,  #SUCCESS
        ret
op_el2:
        tlbi            alle2
        mov             x0,  #SUCCESS
        ret
op_el3:
        tlbi            alle3
        mov             x0,  #SUCCESS
        ret

        .end

