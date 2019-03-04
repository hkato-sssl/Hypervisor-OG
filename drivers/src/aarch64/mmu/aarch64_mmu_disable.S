/*
 * driver/aarch64/mmu/aarch64_mmu_disable.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"
#include "driver/aarch64/system_register.h"

        .global         aarch64_mmu_disable

        .extern         aarch64_read_sctlr
        .extern         aarch64_write_sctlr

        .section        ".text.aarch64_mmu_disable", "ax", %progbits
        .type           aarch64_mmu_disable, %function
        .balign         4
aarch64_mmu_disable:
        str             lr,  [sp,  #-16]!

        bl              aarch64_read_sctlr

        mov             x1,  #(SCTLR_I | SCTLR_C | SCTLR_M)
        bic             x0,  x0,  x1
        bl              aarch64_write_sctlr

        /* CAUTION:
         * 本関数及び本関数の呼び出し元ではスタック領域を利用している為、
         * MMUを無効化した後は必ずD-$のcleanを実行する必要がある。これを
         * 行わないとMMUを無効化した後でD-$の内容が参照されず、スタック領
         * 域の読み込み処理で不整合が発生する。
         * aarch64_dcache_XXX_all()関数は全てスタック領域を利用しない実装
         * となっているので、メモリとキャッシュのコヒーレンシ状況を気にせ
         * ず実行する事が可能。
         */
        bl              aarch64_dcache_clean_invd_all

        ldr             lr,  [sp],  #16
        b               aarch64_icache_invd_all

        .end

