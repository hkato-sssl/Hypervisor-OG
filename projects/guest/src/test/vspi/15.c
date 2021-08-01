/*
 * test/vspi/15.c
 *
 * (C) 2020 Hidekazu Kato
 */

/*
 * テスト内容：LOWEST PRIORITY/ASSERT/HIGHEST PRIORITY（シングル／マルチ）
 * 実行手順
 *   IRQ有効化／PMRマスクなし
 *   全vSPI最低優先度設定
 *   全vSPIをアサート
 *   GICD_IPRIORITYRnを１ワードずつ0xff10ff10を書き込む
 *   割り込み番号の小さい順にレジスタ設定を行う
 *   マルチではVPC#N(N>0)にて全手順を実行
 * 確認事項（シングル）
 *   0x00に優先度を変更した順でvSPIを受信
 * 確認事項（マルチ）
 *   優先度を0に設定した全vSPIの割り込みが通知される
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
#include "driver/arm/gic400_io.h"
#include "driver/arm/device/gic400.h"
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
    vspi_init_interrupts(0x0f);
}

static void set_priority(uint32_t d)
{
    int i;
    uint8_t *reg;

    reg = gic400_distributor_register_base(&gic);
    reg += GICD_IPRIORITYR((gic.nr_interrupts - 32) / 4);
    for (i = 0; i < 8; ++i) {
        REG_WRITE32(reg, 0, d);
        reg += 4;
    }
    memory_barrier_sync();
}

static void assert_15(void)
{
    uint32_t no;

    for (no = gic.nr_interrupts - 32; no < gic.nr_interrupts; ++no) {
        vspi_assert_interrupt(no);
    }
}

static void vspi_15(void)
{
    printk("<%s>\n", __func__);

    initialize();

    gic400_dump_ns_cpuif(&gic);
    gic400_dump_ns_distributor(&gic);

    printk("Start.\n");
    vspi_set_start(true);

    gic400_set_priority_mask(&gic, 0xff);
    aarch64_enable_irq();

    assert_15();
    set_priority(0xff10ff10);

    gic400_dump_ns_cpuif(&gic);
    gic400_dump_ns_distributor(&gic);

    printk("<%s> Done.\n", __func__);
    vspi_set_start(false);

    __asm volatile ("hvc #0");
}

static void vspi_15_primary(void)
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

static void vspi_15_secondary(void)
{
    while (! vspi_get_start()) {
        arm_wfe();
    }

    printk("<%s>\n", __func__);

    assert_15();
    set_priority(0xff10ff10);
    vspi_set_start(false);
}

void test_vspi_15(void)
{
    if (cpu_no() != 0) {
        return;
    }

    vspi_15();
}

void test_vspi_15m(void)
{
    if (cpu_no() == 0) {
        vspi_15_primary();
    } else if (cpu_no() == 1) {
        vspi_15_secondary();
    }
}

