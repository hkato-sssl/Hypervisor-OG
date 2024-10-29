// SPDX-License-Identifier: Apache-2.0
/*
 * test/fp/01.c
 *
 * (C) 2020 Hidekazu Kato
 */

/* テスト内容：
 * Advanced SIMD/FP関連のレジスタアクセスが正常に実行される事を確認する。
 * 処理内容は問題では無い。Guest環境上で適切なリソースへのアクセスが可能
 * である事を確認する。
 */

#include "driver/aarch64.h"
#include "driver/aarch64/system_register.h"
#include "lib/system/printk.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static void enable_fp(void)
{
    uint32_t d;

    d = aarch64_read_cpacr_el1();
    d |= BITS(21, 20);
    aarch64_write_cpacr_el1(d);
    aarch64_isb();
}

void test_fp_01(void)
{
    uint64_t buff[2];

    printk("<%s>\n", __func__);

    enable_fp();

    __asm volatile("ldr q0, [%0]" ::"r"(buff));

    printk("Done.\n");
}
