/*
 * driver/aarch64/mmu/aarch64_mmu_enable_asm.S
 *
 * (C) 2019 Hidekazu Kato
 */

 #include "lib/asm.h"
 #include "lib/system/errno.h"
 #include "driver/aarch64/system_register.h"

        .global         aarch64_mmu_enable_asm

        .extern         aarch64_mmu_set_translation_table
        .extern         aarc64_icache_invd_all
        .extern         aarc64_dcache_invd_all
        .extern         aarch64_read_sctlr
        .extern         aarch64_write_sctlr

        .section        ".text.aarch64_mmu_enable_asm", "ax", %progbits
        .type           aarch64_mmu_enable_asm, %function
        .balign         4
aarch64_mmu_enable_asm:
        str             lr,  [sp, #-16]!
        bl              aarch64_dcache_invd_all
        ldr             lr,  [sp], #16

        BR_EL           0f
0:
        .quad           error
        .quad           el1
        .quad           el2
        .quad           el3
el1:
        /* disable and invalidate I-$ */
        mrs             x0, SCTLR_EL1
        bic             x1,  x0,  #SCTLR_I
        dsb             sy
        isb
        msr             SCTLR_EL1, x1
        isb
        ic              IALLU
        dsb             ish

        /* invalidate all TLB */
        tlbi            vmalle1
        dsb             ish

        /* enable I-$, D-$ and MMU */
        mov             x1,  #(SCTLR_I | SCTLR_C | SCTLR_M)
        orr             x0,  x0,  x1

        isb
        msr             SCTLR_EL1, x0
        isb
        b               success
el2:
        /* disable and invalidate I-$ */
        mrs             x0, SCTLR_EL2
        bic             x1,  x0,  #SCTLR_I
        dsb             sy
        isb
        msr             SCTLR_EL2, x1
        isb
        ic              IALLU
        dsb             ish

        /* invalidate all TLB */
        tlbi            alle2
        dsb             ish

        /* enable I-$, D-$ and MMU */
        mov             x1,  #(SCTLR_I | SCTLR_C | SCTLR_M)
        orr             x0,  x0,  x1

        isb
        msr             SCTLR_EL2, x0
        isb
        b               success
el3:
        /* disable and invalidate I-$ */
        mrs             x0, SCTLR_EL3
        bic             x1,  x0,  #SCTLR_I
        dsb             sy
        isb
        msr             SCTLR_EL3, x1
        isb
        ic              IALLU
        dsb             ish

        /* invalidate all TLB */
        tlbi            alle3
        dsb             ish

        /* enable I-$, D-$ and MMU */
        mov             x1,  #(SCTLR_I | SCTLR_C | SCTLR_M)
        orr             x0,  x0,  x1

        isb
        msr             SCTLR_EL3, x0
        isb
success:
        mov             x0,  xzr
        ret
error:
        mov             x0,  #-EBUSY
        ret

        .end

