// SPDX-License-Identifier: Apache-2.0
/*
 * test/aarch64/cache/01_clean.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "cache_test.h"
#include "driver/aarch64/cache.h"
#include "driver/aarch64/mmu.h"
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/system_register/mair_elx.h"
#include "driver/aarch64/system_register/tcr_elx.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "lib/system/printk.h"
#include <stdint.h>
#include <string.h>

/* テスト項目：Clean VA
 *
 * 1) 同じ物理メモリ領域をVA1/VA2の２つにマッピング
 * 2) VA1/VA2共にWrite-back(Read-allocate)でマッピング
 * 3) １度VA1/VA2の値を読み込む
 * 4) VA1に書き込んだ値がVA2から参照可能である事を確認
 * 5) PAでは値が変更されていない事を確認
 * 6) VA1に対してcleanを実行
 * 7) PAの値がVA1/VA2と同じ値になる事を確認
 * 8) VA1とPAに異なる値を書き込む
 * 9) VA1に対してcleanを実行
 * 10) PAの値がVA1/VA2と同じ値になる事を確認
 *
 * 前提条件：
 * 1) MMU無効状態でテストコードを実行
 * 2) Cacheの影響を排除する為にbss/data/stack領域全てをNon-cacheable
 *    でマッピングする
 * 3) EL2で実行
 *    EL1はEL2/EL3による設定でキャッシュ保守操作自体の挙動が変更され
 *    る為、MMU/Cache driverの動作確認には適さない。
 */

/* defines */

/* test parameters */

#define TEST_VA1 0xe0000000
#define TEST_VA2 0xe0010000

/* types */

/* prototypes */

/* variables */

extern struct aarch64_mmu test_mmu;

static char test_region[4096] __attribute__((aligned(4096)));

/* functions */

errno_t test_aarch64_cache_01_clean(void)
{
    errno_t ret;
    volatile char *va1;
    volatile char *va2;
    struct aarch64_mmu_attr attr;

    printk("<%s>\n", __func__);

    ret = test_aarch64_cache_init_memory_map();
    printk("test_aarch64_cache_init_memory_map() -> %d\n", ret);
    if (ret != SUCCESS) {
        return ret;
    }

    memset(&attr, 0, sizeof(attr));
    attr.af = 1;
    attr.sh = MMU_ATTR_SH_ISH;
    attr.ap21 = MMU_ATTR_AP_RW;
    attr.attrindx = MMU_ATTR_WB;

    va1 = (void *)TEST_VA1;
    va2 = (void *)TEST_VA2;

    ret = aarch64_mmu_map(&test_mmu, (void *)va1, test_region,
                          sizeof(test_region), &attr);
    printk("VA1: aarch64_mmu_map() -> %d\n", ret);
    if (ret != SUCCESS) {
        return ret;
    }

    ret = aarch64_mmu_map(&test_mmu, (void *)va2, test_region,
                          sizeof(test_region), &attr);
    printk("VA2: aarch64_mmu_map() -> %d\n", ret);
    if (ret != SUCCESS) {
        return ret;
    }

    memset(test_region, 0, sizeof(test_region));
    printk(" PA[0]: 0x%02x\n", test_region[0]);
    printk("VA1[0]: 0x%02x\n", va1[0]);
    printk("VA2[0]: 0x%02x\n", va2[0]);
    printk("\n");

    printk("0x11 -> VA1[0]\n");
    va1[0] = 0x11;
    printk("VA1[0]: 0x%02x\n", va1[0]);
    printk("VA2[0]: 0x%02x\n", va2[0]);
    printk(" PA[0]: 0x%02x\n", test_region[0]);
    printk("\n");

    printk("Clean VA1.\n");
    aarch64_dcache_clean((void *)va1);
    printk("VA1[0]: 0x%02x\n", va1[0]);
    printk("VA2[0]: 0x%02x\n", va2[0]);
    printk(" PA[0]: 0x%02x\n", test_region[0]);
    printk("\n");

    printk("0x77 -> PA[0]\n");
    printk("0x88 -> VA1[0]\n");
    test_region[0] = 0x77;
    memory_barrier();
    va1[0] = 0x88;
    printk(" PA[0]: 0x%02x\n", test_region[0]);
    printk("VA1[0]: 0x%02x\n", va1[0]);
    printk("VA2[0]: 0x%02x\n", va2[0]);
    printk("\n");

    printk("clean VA1.\n");
    aarch64_dcache_clean((void *)va1);
    printk(" PA[0]: 0x%02x\n", test_region[0]);
    printk("VA1[0]: 0x%02x\n", va1[0]);
    printk("VA2[0]: 0x%02x\n", va2[0]);
    printk("\n");

    printk("Done\n");

    return SUCCESS;
}
