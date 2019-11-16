/*
 * driver/aarch64/mmu/aarch64_mmu_disable.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"
#include "lib/system/errno.h"
#include "driver/aarch64/system_register.h"

        .global         aarch64_mmu_disable

        .extern         aarch64_read_sctlr
        .extern         aarch64_write_sctlr
        .extern         aarch64_icache_invd_all
        .extern         aarch64_dcache_clean_invd_all

        .section        ".text.aarch64_mmu_disable", "ax", %progbits
        .type           aarch64_mmu_disable, %function

        /* CAUTION:
         * 本関数及び本関数の呼び出し元ではスタック領域を利用している為、
         * MMUを無効化する際に必ずD-$のcleanを実行する必要がある。これを
         * 行わないとMMUを無効化した後で、スタック領域の読み込み処理で不
         * 整合が発生する可能性がある。
         * Cacheドライバが用意する保守APIは全てスタック領域を利用しない実
         * 装となっているので、メモリとキャッシュのコヒーレンシ状況を気に
         * せず実行する事が可能。
         */
        .balign         4
aarch64_mmu_disable:
        stp             x19, lr,  [sp,  #-16]!

        bl              aarch64_read_sctlr
        tbz             x0,  #0,  error     // test SCTLR.M
	mov             x19, x0

        bl              aarch64_dcache_clean_invd_all

        mov             x1,  #(SCTLR_C | SCTLR_M)
        bic             x0,  x19, x1
        bl              aarch64_write_sctlr

        bl              aarch64_icache_invd_all
        mov             x0,  #SUCCESS
exit:
        ldp             x19, lr,  [sp],  #16
        ret
error:
        mov             x0,  #-EPERM
        b               exit

        .end

