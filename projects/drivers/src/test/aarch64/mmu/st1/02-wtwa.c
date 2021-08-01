/*
 * test/aarch64/mmu/st1/02-wtwa.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/system_register/tcr_elx.h"
#include "driver/aarch64/system_register/mair_elx.h"
#include "driver/aarch64/mmu.h"
#include "st1.h"

/* テスト項目：ミラーリング(Write-through, Read-allocate, Write-allocate)
 *
 * 1) 同じ物理メモリ領域をVA1/VA2の２つにマッピング
 * 2) VA1/VA2共にWrite-throughでマッピング
 * 3) VA1に書き込んだ値がVA2から参照可能である事を確認
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

#define TEST_VA1                0xe0000000
#define TEST_VA2                0xe0010000

/* types */

/* prototypes */

/* variables */

extern struct aarch64_mmu test_mmu;

static char test_region[4096] __attribute__((aligned(4096)));

/* functions */

errno_t test_aarch64_mmu_st1_02_wtwa(void)
{
    errno_t ret;
    char *va1;
    char *va2;
    struct aarch64_mmu_attr attr;

    printk("<%s>\n", __func__);

    ret = test_aarch64_mmu_st1_init_memory_map();
    printk("test_aarch64_mmu_st1_init_memory_map() -> %d\n", ret);
    if (ret != SUCCESS) {
        return ret;
    }

    memset(&attr, 0, sizeof(attr));
    attr.af = 1;
    attr.sh = MMU_ATTR_SH_ISH;
    attr.ap21 = MMU_ATTR_AP_RW;
    attr.attrindx = MMU_ATTR_WTWA;

    va1 = (void *)TEST_VA1;
    va2 = (void *)TEST_VA2;

    ret = aarch64_mmu_map(&test_mmu, va1, test_region, sizeof(test_region), &attr);
    printk("VA1: aarch64_mmu_map() -> %d\n", ret);
    if (ret != SUCCESS) {
        return ret;
    }

    ret = aarch64_mmu_map(&test_mmu, va2, test_region, sizeof(test_region), &attr);
    printk("VA2: aarch64_mmu_map() -> %d\n", ret);
    if (ret != SUCCESS) {
        return ret;
    }
    
    memset(va1, 0, sizeof(test_region));
    printk("va1[0]=%d\n", va1[0]);
    printk("va2[0]=%d\n", va2[0]);
    printk("\n");
    printk("++va1[0]=%d\n", ++va1[0]);
    printk("  va2[0]=%d\n", va2[0]);
    printk("   pa[0]=%d\n", test_region[0]);
    printk("\n");
    printk("++va2[0]=%d\n", ++va2[0]);
    printk("  va1[0]=%d\n", va1[0]);
    printk("   pa[0]=%d\n", test_region[0]);
    printk("\n");
    printk("write 100 to pa[0].\n");
    test_region[0] = 100;
    printk("va1[0]=%d\n", va1[0]);
    printk("va2[0]=%d\n", va2[0]);
    printk("\n");
    printk("va1[256]=%d\n", va1[256]);
    printk("va2[256]=%d\n", va2[256]);
    printk("write 77 to pa[256].\n");
    test_region[256] = 77;
    printk("va1[256]=%d\n", va1[256]);
    printk("va2[256]=%d\n", va2[256]);

    printk("Done\n");

    return SUCCESS;
}

