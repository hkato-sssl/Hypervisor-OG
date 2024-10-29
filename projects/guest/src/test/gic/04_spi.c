// SPDX-License-Identifier: Apache-2.0
/*
 * test/gic/04_spi.c
 *
 * (C) 2020 Hidekazu Kato
 */

/*
 * テスト内容：
 * 割り込み可能状態で任意のSPIをAssertする。
 * SPIを受信すれば成功とする。
 */

#include "driver/aarch64.h"
#include "driver/arm/gic400.h"
#include "driver/system/cpu.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include <stdint.h>
#include <string.h>

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

static void assert_interrupt(uint32_t no)
{
    errno_t ret;

    ret = gic400_assert_spi(&gic, no);
    printk("gic400_assert_interrupt(%u) -> %d\n", no, ret);
}

static void init_interrupt(void)
{
    errno_t ret;
    uint16_t i;
    struct gic400_interrupt_configuration config;

    interrupt_handler = test_handler;

    memset(&config, 0, sizeof(config));
    config.targets = 1;
    config.priority = 2;
    config.flag.edge = 0;

    for (i = 32; i < gic.nr_interrupts; ++i) {
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

void test_gic_04(void)
{
    int i;

    printk("<%s>\n", __func__);

    init_interrupt();

    gic400_dump_ns_cpuif(&gic);
    gic400_dump_ns_distributor(&gic);

    gic400_set_priority_mask(&gic, 0xff);
    assert_interrupt(130);
    assert_interrupt(131);
    assert_interrupt(165);
    for (i = 1; i <= 8; ++i) {
        assert_interrupt(gic.nr_interrupts - i);
    }

    printk("\n\n---------------------\n\n");

    gic400_dump_ns_cpuif(&gic);
    gic400_dump_ns_distributor(&gic);

    printk("<%s> Done.\n", __func__);
    __asm volatile("hvc #0");
}
