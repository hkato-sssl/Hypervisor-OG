/*
 * aarch64/mmu/aarch64_mmu_write_tt.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         aarch64_mmu_write_tt

        .section        ".text.aarch64_mmu_write_tt", "ax", %progbits
        .type           aarch64_mmu_write_tt, %function
        .balign         4
aarch64_mmu_write_tt:
        str             x1,  [x0]
        dsb             ish
        dc              cvau, x0
        dsb             ish
        ret

        .end

