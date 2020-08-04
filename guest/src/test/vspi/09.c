/*
 * test/vspi/09.c
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
 *   割り込み番号の小さい順に有効化する
 *   マルチではVPC#N(N>0)にて全手順を実行
 * 確認事項（シングル）
 *   有効化した順で割り込みが通知される
 * 確認事項（マルチ）
 *   全vSPIの割り込みが通知される
 * 注意事項
 *   マルチ環境ではHypervisor内部でVCPU#1からVCPU#0へイベント送信が発生
 *   する。VCPU#0での処理に遅延が発生する事が避けられない為、Guest上での
 *   レジスタ操作と実際に通知される割り込みは必ずしも一致しない事があり
 *   得る。
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
#include "vspi.h"

/* defines */

/* types */

/* prototypes */

/* variables */

extern struct gic400 gic;

/* functions */

static void initialize(void)
{
    vspi_set_start(false);
    vspi_init_interrupts();
}

static void assert_09(void)
{
    uint32_t no;

    for (no = gic.nr_interrupts - 32; no < gic.nr_interrupts; ++no) {
        gic400_disable_interrupt(&gic, no);
    }

    for (no = gic.nr_interrupts - 32; no < gic.nr_interrupts; ++no) {
        vspi_assert_interrupt(no);
    }
}

static void enable_09(void)
{
    uint32_t no;

    for (no = gic.nr_interrupts - 32; no < gic.nr_interrupts; ++no) {
        gic400_enable_interrupt(&gic, no);
    }
}

static void vspi_09(void)
{
    printk("<%s>\n", __func__);

    initialize();

    gic400_dump_ns_cpuif(&gic);
    gic400_dump_ns_distributor(&gic);

    printk("Start.\n");
    vspi_set_start(true);

    gic400_set_priority_mask(&gic, 0xff);
    aarch64_enable_irq();

    assert_09();
    enable_09();

    gic400_dump_ns_cpuif(&gic);
    gic400_dump_ns_distributor(&gic);

    printk("<%s> Done.\n", __func__);
    vspi_set_start(false);

    __asm volatile ("hvc #0");
}

static void vspi_09_primary(void)
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

    __asm volatile ("hvc #0");
}

static void vspi_09_secondary(void)
{
    while (! vspi_get_start()) {
        arm_wfe();
    }

    printk("<%s>\n", __func__);
    assert_09();
    enable_09();
    vspi_set_start(false);
}

void test_vspi_09(void)
{
    if (cpu_no() != 0) {
        return;
    }

    vspi_09();
}

void test_vspi_09m(void)
{
    if (cpu_no() == 0) {
        vspi_09_primary();
    } else if (cpu_no() == 1) {
        vspi_09_secondary();
    }
}

