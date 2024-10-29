// SPDX-License-Identifier: Apache-2.0
/*
 * test/vspi/08.c
 *
 * (C) 2020 Hidekazu Kato
 */

/*
 * テスト内容：DISABLE/ASSERT/ENABLE（シングル／マルチ）
 * 実行手順
 *   IRQ有効化／PMRマスクなし
 *   全vSPI無効化
 *   全vSPIをアサート
 *   １つずつvSPIを有効化する
 *   マルチではVPC#N(N>0)にて全手順を実行
 * 確認事項
 *   有効化した順でvSPIを受信
 */

#include "driver/aarch64.h"
#include "driver/arm.h"
#include "driver/arm/gic400.h"
#include "driver/system/cpu.h"
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "lib/system/printk.h"
#include "vspi.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* defines */

/* types */

/* prototypes */

/* variables */

extern struct gic400 gic;

/* functions */

static void initialize(void)
{
    vspi_set_start(false);
    vspi_init_interrupts(8);
}

static void assert_08(void)
{
    uint32_t no;

    for (no = gic.nr_interrupts - 32; no < gic.nr_interrupts; ++no) {
        gic400_disable_interrupt(&gic, no);
    }

    for (no = gic.nr_interrupts - 32; no < gic.nr_interrupts; ++no) {
        vspi_assert_interrupt(no);
    }
}

static void enable_08(void)
{
    uint32_t no;

    for (no = gic.nr_interrupts - 32; no < gic.nr_interrupts; ++no) {
        gic400_enable_interrupt(&gic, no);
    }
}

static void vspi_08(void)
{
    printk("<%s>\n", __func__);

    initialize();

    gic400_dump_ns_cpuif(&gic);
    gic400_dump_ns_distributor(&gic);

    printk("Start.\n");
    vspi_set_start(true);

    gic400_set_priority_mask(&gic, 0xff);
    aarch64_enable_irq();

    assert_08();
    enable_08();

    gic400_dump_ns_cpuif(&gic);
    gic400_dump_ns_distributor(&gic);

    printk("<%s> Done.\n", __func__);
    vspi_set_start(false);

    __asm volatile("hvc #0");
}

static void vspi_08_primary(void)
{
    printk("<%s>\n", __func__);

    initialize();

    gic400_dump_ns_cpuif(&gic);
    gic400_dump_ns_distributor(&gic);

    printk("Start.\n");
    vspi_set_start(true);

    gic400_set_priority_mask(&gic, 0xff);
    aarch64_enable_irq();

    while (vspi_get_start()) {
        arm_wfe();
    }

    gic400_dump_ns_cpuif(&gic);
    gic400_dump_ns_distributor(&gic);

    printk("<%s> Done.\n", __func__);
    vspi_set_start(false);

    __asm volatile("hvc #0");
}

static void vspi_08_secondary(void)
{
    while (! vspi_get_start()) {
        arm_wfe();
    }

    printk("<%s>\n", __func__);
    assert_08();
    enable_08();
    vspi_set_start(false);
}

void test_vspi_08(void)
{
    if (cpu_no() != 0) {
        return;
    }

    vspi_08();
}

void test_vspi_08m(void)
{
    if (cpu_no() == 0) {
        vspi_08_primary();
    } else if (cpu_no() == 1) {
        vspi_08_secondary();
    }
}
