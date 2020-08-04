/*
 * test/vspi/03.c
 *
 * (C) 2030 Hidekazu Kato
 */

/*
 * テスト内容：マスク／アサート／アンマスク１
 * 実行手順
 *   IRQ無効化／PMRマスクなし
 *   全vSPIを割り込み番号の小さい順にアサート
 *   IRQ有効化
 *   マルチではVPC#N(N>0)からVPC#0へアサートする
 * 確認事項
 *   vSPIを優先度の高い（番号の小さい）順に受信
 *   マルチコア環境では割り込みを全て受信する事を優先する
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "lib/system/printk.h"
#include "driver/arm/gic400.h"
#include "driver/arm.h"
#include "driver/aarch64.h"
#include "driver/system/cpu.h"

/* defines */

/* types */

/* prototypes */

/* variables */

extern void (*interrupt_handler)(uint16_t vector);
extern struct gic400 gic;

static volatile bool start = false;

/* functions */

static void set_start(bool v)
{
    memory_barrier();
    start = v;
    memory_barrier_sync();
    arm_sev();
}

static void test_handler(uint16_t vector)
{
    uint32_t iar;

    printk("<%s> vector=0x%04x\n", __func__, vector);

    iar = gic400_ack(&gic);
    printk("IAR = 0x%08x\n", iar);
    gic400_eoi(&gic, iar);
}

static void init_interrupt(void)
{
    errno_t ret;
    uint16_t i;
    struct gic400_interrupt_configuration config;

    aarch64_disable_irq();
    gic400_set_priority_mask(&gic, 0);

    interrupt_handler = test_handler;

    memset(&config, 0, sizeof(config));
    config.targets = 1;
    config.priority = 2;
    config.flag.edge = 0;

    for (i = 0; i < gic.nr_interrupts; ++i) {
        ret = gic400_configure_interrupt(&gic, i, &config);
        if (ret != SUCCESS) {
            printk("gic400_configure_interrupt(%u) -> %d\n", i, ret);
        }

        ret = gic400_enable_interrupt(&gic, i);
        if (ret != SUCCESS) {
            printk("gic400_enable_interrupt(%u) -> %d\n", i, ret);
        }
    }
}

static void initialize(void)
{
    set_start(false);
    init_interrupt();
}

static void assert_interrupt(uint32_t no)
{
    errno_t ret;

    if (no < 16) {
        ret = gic400_assert_sgi(&gic, 1, no);
        printk("gic400_assert_sgi(%u) -> %d\n", no, ret);
    } else if (no >= 32) {
        ret = gic400_assert_spi(&gic, no);
        printk("gic400_assert_spi(%u) -> %d\n", no, ret);
    }
}

static void assert_03(void)
{
    uint32_t no;

    for (no = gic.nr_interrupts - 32; no < gic.nr_interrupts; ++no) {
        assert_interrupt(no);
    }
}

static void vspi_03(void)
{
    printk("<%s>\n", __func__);

    initialize();

    gic400_dump_ns_cpuif(&gic);
    gic400_dump_ns_distributor(&gic);

    printk("Start.\n");
    set_start(true);

    gic400_set_priority_mask(&gic, 0xff);

    assert_03();

    aarch64_enable_irq();

    gic400_dump_ns_cpuif(&gic);
    gic400_dump_ns_distributor(&gic);

    printk("<%s> Done.\n", __func__);
    set_start(false);

    __asm volatile ("hvc #0");
}

static void vspi_03_primary(void)
{
    printk("<%s>\n", __func__);

    initialize();

    gic400_dump_ns_cpuif(&gic);
    gic400_dump_ns_distributor(&gic);

    printk("Start.\n");
    set_start(true);

    gic400_set_priority_mask(&gic, 0xff);

    while (start) {
        arm_wfe();
    }

    aarch64_enable_irq();

    gic400_dump_ns_cpuif(&gic);
    gic400_dump_ns_distributor(&gic);

    printk("<%s> Done.\n", __func__);
    set_start(false);

    __asm volatile ("hvc #0");
}

static void vspi_03_secondary(void)
{
    while (! start) {
        arm_wfe();
    }

    printk("<%s>\n", __func__);
    assert_03();
    set_start(false);
}

void test_vspi_03(void)
{
    if (cpu_no() != 0) {
        return;
    }

    vspi_03();
}

void test_vspi_03m(void)
{
    if (cpu_no() == 0) {
        vspi_03_primary();
    } else if (cpu_no() == 1) {
        vspi_03_secondary();
    }
}

