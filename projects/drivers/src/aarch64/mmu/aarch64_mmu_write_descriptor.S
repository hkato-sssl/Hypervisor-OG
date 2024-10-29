// SPDX-License-Identifier: Apache-2.0
/*
 * aarch64/mmu/aarch64_mmu_write_descriptor.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         aarch64_mmu_write_descriptor

        .section        ".text.aarch64_mmu_write_descriptor", "ax", %progbits
        .type           aarch64_mmu_write_descriptor, %function
        .balign         4
        /*
         * x0 - descriptor address
         * x1 - data
         *
         * CAUTION:
         * 本ドライバはSMMU descriptorを扱う事もある為、キャッシュ保守操作は
         * PoC まで実行する必要がある。
         */
aarch64_mmu_write_descriptor:
		dsb				sy
        str             x1, [x0]
        dsb             ish
        dc              cvac, x0
        dsb             osh
        ret

        .end

