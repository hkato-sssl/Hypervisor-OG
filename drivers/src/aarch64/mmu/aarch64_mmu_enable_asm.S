/*
 * driver/aarch64/mmu/aarch64_mmu_enable_asm.S
 *
 * (C) 2019 Hidekazu Kato
 */

 #include "lib/asm.h"
 #include "lib/system/errno.h"
 #include "driver/aarch64/system_register.h"

        .global         aarch64_mmu_enable_asm

        .extern         aarc64_icache_invd_all
        .extern         aarc64_dcache_invd_all
        .extern         aarch64_read_sctlr
        .extern         aarch64_write_sctlr
        .extern         aarch64_write_ttbr0

        .section        ".text.aarch64_mmu_enable_asm", "ax", %progbits
        .type           aarch64_mmu_enable_asm, %function
        .balign         4
        /*
         * x0 - TTBR0
         * x1 - ASID
         */
aarch64_mmu_enable_asm:
        stp             x20, lr,  [sp,  #-16]!
        mov             x20, x0

        bl              aarch64_read_sctlr
        mov             x1,  #(SCTLR_I | SCTLR_C | SCTLR_M)
        tst             x0,  x1
        bne             error

        orr             x1,  x0,  x1                // new SCTLR
        mov             x0,  x20
        mov             x20, x1
        bl              aarch64_mmu_set_ttbr0
        cbnz            x0,  exit

        bl              aarch64_mmu_tlbi_all
        bl              aarch64_icache_invd_all
        bl              aarch64_dcache_invd_all

        mov             x0,  x20                    // new SCTLR
        bl              aarch64_write_sctlr

        mov             x0,  #SUCCESS
exit:
        ldp             x20, lr,  [sp],  #16
        ret
error:
        mov             x0,  #-EBUSY
        b               exit

        .end

