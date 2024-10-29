// SPDX-License-Identifier: Apache-2.0
/*
 * test/insn/03_parse_aarch64_ldr_literal.c
 *
 * (C) 2020 Hidekazu Kato
 */

/* テスト内容：LDR Xn|Wn, literal形式の解析結果確認
 *
 * Hypervisorがエミュレーションをサポートするメモリアクセス命令を実行した
 * 際の解析結果を確認する。実行内容に意味は無い。
 * メモリアクセスによるトラップを発生させる為にアクセスするアドレスは
 * Hypervisor領域とする。
 *
 * 1.解析対象命令
 * 解析対象となる命令はLDR命令のliteral指定形式となる。
 * literal形式の実行にはメモリマッピングの調整が必要となる為、Hypervisor
 * 側にもテストコードを用意する。
 *
 * LDR (64-bit)
 * ・LDR Xn, <label>
 *
 * LDR (32-bit)
 * ・LDR Wn, <label>
 *
 * 2.テスト実施方針
 * Region Trap発生時に命令を解析した結果が正しいか否かを判断する。
 */

#include "driver/aarch64/cache.h"
#include "driver/aarch64/mmu.h"
#include "lib/system/printk.h"
#include "system/mmu.h"
#include <stdint.h>
#include <string.h>

#define ACCESS_REGION 0x40000000
#define MAP_ADDR      (ACCESS_REGION - 4096)
#define CODE_RET      0xd65f03c0 /* 'ret' instruction. */
#define CODE_NOP      0xd503201f /* 'nop' instruction. */

extern struct aarch64_mmu sys_mmu;
extern const uint32_t test_insn_03_ldr[];
extern const uint32_t test_insn_03_ldrw[];

static uint8_t buff[4096] __attribute__((aligned(4096)));

typedef void (*test_func_t)(void);

static void clean_buff(void)
{
    memset(buff, 0, sizeof(buff));
    aarch64_dcache_clean_invd_range(buff, sizeof(buff));
}

static void map(void)
{
    errno_t ret;
    struct aarch64_mmu_attr attr;

    memset(&attr, 0, sizeof(attr));
    attr.af = 1;
    attr.sh = MMU_ATTR_SH_ISH;
    attr.ap21 = MMU_ATTR_AP_RW;
    attr.attrindx = EL1_MMU_WB;
    ret = aarch64_mmu_map(&sys_mmu, (void *)MAP_ADDR, buff, 4096, &attr);
    printk("aarch64_mmu_map() -> %d\n", ret);

    ret = aarch64_mmu_map(&sys_mmu, (void *)ACCESS_REGION,
                          (void *)ACCESS_REGION, 4096, &attr);
    printk("aarch64_mmu_map() -> %d\n", ret);
}

static void test_insn(int n, const uint32_t *insn)
{
    uint32_t *p = (void *)buff;
    test_func_t f = (test_func_t)MAP_ADDR;

    p[0] = insn[n];
    p[1] = CODE_RET;
    aarch64_dcache_clean(buff);
    aarch64_icache_invd_all();
    (f)();
}

static void test_ldr(int n)
{
    test_insn(n, test_insn_03_ldr);
}

static void test_ldrw(int n)
{
    test_insn(n, test_insn_03_ldrw);
}

static void pattern_01(void)
{
    int i;

    printk("  %s()\n", __func__);

    for (i = 0; i < 32; ++i) {
        test_ldr(i);
    }
    for (i = 0; i < 32; ++i) {
        test_ldrw(i);
    }
}

static void pattern_02(void)
{
    int i;
    uint32_t *p = (uint32_t *)buff;
    test_func_t f = (test_func_t)MAP_ADDR;

    printk("  %s()\n", __func__);

    for (i = 0; i < 32; ++i) {
        *p++ = test_insn_03_ldr[i];
        *p++ = CODE_NOP;
    }
    for (i = 0; i < 32; ++i) {
        *p++ = test_insn_03_ldrw[i];
    }
    *p = CODE_RET;
    aarch64_dcache_clean_range(buff, sizeof(buff));
    aarch64_icache_invd_all();
    (f)();
}

void test_insn_03(void)
{
    printk("%s()\n", __func__);
    clean_buff();
    map();
    pattern_01();
    pattern_02();
    printk("Done.\n");
}
