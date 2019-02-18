/*
 * aarch64/mmu/aarch64_mmu_tlbi_va.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/bit.h"
#include "lib/aarch64.h"

        .global         aarch64_mmu_tlbi_va

        .section        ".text.aarch64_mmu_tlbi_va", "ax", %progbits
        .type           aarch64_mmu_tlbi_va, %function
        .balign         4
aarch64_mmu_tlbi_va:
        BR_EL           branch_table

el0:
        /* no operation */
        ret
el1:
        tlbi            vae1is, x0
        ret
el2:
        tlbi            vae2is, x0
        ret
el3:
        tlbi            vae3is, x0
        ret

branch_table:
        .quad           el0
        .quad           el1
        .quad           el2
        .quad           el3

        .end

