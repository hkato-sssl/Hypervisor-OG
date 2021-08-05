/*
 * test/aarch64/mmu/st2/01.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/aarch64/stage2.h"
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/system_register/vtcr_el2.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "st2.h"
#include <stdint.h>
#include <string.h>

/* テスト項目：stage 2 translation mapping
 *
 * 1) RAMとUART liteに関するマッピングを実施
 * 2) ダンプ情報出力
 *
 * 前提条件：
 *
 * 1) Stage 1 には影響を与えない
 */

/* defines */

/* test parameters */

#define UART_IPA 0xa0001000
#define UART_PA  0xafff0000

/* types */

/* prototypes */

/* variables */

extern struct aarch64_stage2 test_st2_mmu;

/* functions */

errno_t test_aarch64_mmu_st2_01(void)
{
    errno_t ret;
    struct aarch64_stage2_attr attr;

    printk("<%s>\n", __func__);

    ret = test_aarch64_stage2_init_memory_map();
    printk("test_aarch64_stage2_init_memory_map() -> %d\n", ret);
    if (ret != SUCCESS) {
        return ret;
    }

    memset(&attr, 0, sizeof(attr));
    attr.xn = 1;
    attr.af = 1;
    attr.sh = STAGE2_SH_OSH;
    attr.s2ap = STAGE2_S2AP_RW;
    attr.memattr = STAGE2_MEMATTR_DEVICE_nGnRnE;
    ret = aarch64_stage2_map(&test_st2_mmu, (void *)UART_IPA, (void *)UART_PA,
                             4096, &attr);
    printk("aarch64_stage2_map() -> %d\n", ret);
    aarch64_stage2_dump_descriptor(&test_st2_mmu);

    printk("Done\n");

    return SUCCESS;
}
