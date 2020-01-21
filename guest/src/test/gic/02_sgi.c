/*
 * test/gic/02_sgic.c
 *
 * (C) 2020 Hidekazu Kato
 */

/*
 * テスト内容：
 * 任意のSGIを自身に対してAssertし、その割り込み処理を実行する。
 * 複数回のSGI assertが出来れば成功とする。
 */

#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "driver/arm/gic400.h"
#include "driver/aarch64.h"

/* defines */

/* types */

/* prototypes */

/* variables */

extern void (*interrupt_handler)(uint16_t vector);
extern struct gic400 gic;

/* functions */

static void test_handler(uint16_t vector)
{
    uint32_t iar;

    printk("<%s> vector=0x%04x\n", __func__, vector);

    iar = gic400_ack(&gic);
    printk("IAR = 0x%08x\n", iar);
    gic400_eoi(&gic, iar);
}

static void assert_sgi(uint32_t no)
{
    errno_t ret;

    ret = gic400_assert_sgi(&gic, 1, no);
    printk("gic400_assert_sgi(%u) -> %d\n", no, ret);
}

static void init_interrupt(void)
{
    errno_t ret;
    uint16_t i;
    struct gic400_interrupt_configuration config;

    interrupt_handler = test_handler;

    memset(&config, 0, sizeof(config));
    config.targets = 1;
    config.priority = 8;
    config.flag.edge = 1;

    for (i = 0; i < 32; ++i) {
        ret = gic400_configure_interrupt(&gic, i, &config);
        if (ret != SUCCESS) {
            printk("gic400_configure_interrupt(%u) -> %d\n", i, ret);
        }
        
        ret = gic400_enable_interrupt(&gic, i);
        if (ret != SUCCESS) {
            printk("gic400_enable_interrupt(%u) -> %d\n", i, ret);
        }
    }

    gic400_set_priority_mask(&gic, 0xff);
    aarch64_enable_irq();
}

void test_gic_02(void)
{
    uint32_t i;

    printk("<%s>\n", __func__);

    init_interrupt();
    for (i = 0; i < 32; ++i) {
        assert_sgi(i);
    }

    printk("\n\n---------------------\n\n");

    gic400_dump_ns_cpuif(&gic);
    gic400_dump_ns_distributor(&gic);

    printk("<%s> Done.\n", __func__);
}

