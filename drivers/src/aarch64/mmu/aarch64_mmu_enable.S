/*
 * driver/aarch64/mmu/aarch64_mmu_enable.S
 *
 * (C) 2019 Hidekazu Kato
 */

 #include "lib/asm.h"
 #include "lib/system/errno.h"
 #include "driver/aarch64/system_register.h"

        .global         aarch64_mmu_enable

        .extern         aarc64_icache_invd_all
        .extern         aarc64_dcache_invd_all
        .extern         aarch64_read_sctlr
        .extern         aarch64_write_sctlr
        .extern         aarch64_write_ttbr0

        .section        ".text.aarch64_mmu_enable", "ax", %progbits
        .type           aarch64_mmu_enable, %function
        .balign         4
        /*
         * x0 - TTBR0
         * x1 - ASID
         */
aarch64_mmu_enable:
        str             lr,  [sp,  #-16]!
        stp             x19, x20, [sp,  #-16]!

        bfi             x0,  x1,  #48,  #16
        mov             x19, x0                     // TTBR0_ELx

        bl              aarch64_read_sctlr
        mov             x1,  #(SCTLR_I | SCTLR_C | SCTLR_M)
        tst             x0,  x1
        bne             error

        orr             x20,  x0,  x1               // new SCTLR
        bl              aarch64_icache_invd_all
        bl              aarch64_dcache_invd_all

        mov             x0,  x19                    // TTBR0_ELx
        bl              aarch64_write_ttbr0

        mov             x0,  x20                    // new SCTLR
        bl              aarch64_write_sctlr

        mov             x0,  #SUCCESS
exit:
        ldp             x19, x20, [sp],  #16
        ldr             lr,  [sp],  #16
        ret
error:
        mov             x0,  #-EBUSY
        b               exit

        .end

