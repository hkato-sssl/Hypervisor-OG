/*
 * test/vspi/14.c
 *
 * (C) 2020 Hidekazu Kato
 */

/*
 * テスト内容：LOWEST PRIORITY/ASSERT/HIGHEST PRIORITY（シングル／マルチ）
 * 実行手順
 *   IRQ有効化／PMRマスクなし
 *   全vSPI最低優先度設定
 *   全vSPIをアサート
 *   GICD_IPRIORITYRnを１ワードずつ優先度を1に設定
 *   割り込み番号の大きい順にレジスタ設定を行う
 *   マルチではVPC#N(N>0)にて全手順を実行
 * 確認事項（シングル）
 *   優先度を変更した順でvSPIを受信
 * 確認事項（マルチ）
 *   全vSPIの割り込みが通知される
 * 注意事項
 *   マルチ環境ではHypervisor内部でVCPU#1からVCPU#0へイベント送信が発生
 *   する。VCPU#0での処理に遅延が発生する事が避けられない為、Guest上での
 *   レジスタ操作と実際に通知される割り込みは必ずしも一致しない事があり
 *   得る。
 */

#include "driver/aarch64.h"
#include "driver/arm.h"
#include "driver/arm/device/gic400.h"
#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
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
    vspi_init_interrupts(0x0f);
}

static void set_priority(uint8_t d0)
{
    int i;
    uint32_t d;
    uint8_t *reg;

    d = ((uint32_t)d0 << 8) | d0;
    d |= (d << 16);

    reg = gic400_distributor_register_base(&gic);
    reg += GICD_IPRIORITYR(gic.nr_interrupts / 4);
    for (i = 0; i < 8; ++i) {
        reg -= 4;
        REG_WRITE32(reg, 0, d);
    }
    memory_barrier_sync();
}

static void assert_14(void)
{
    uint32_t no;

    for (no = gic.nr_interrupts - 32; no < gic.nr_interrupts; ++no) {
        vspi_assert_interrupt(no);
    }
}

static void vspi_14(void)
{
    printk("<%s>\n", __func__);

    initialize();

    gic400_dump_ns_cpuif(&gic);
    gic400_dump_ns_distributor(&gic);

    printk("Start.\n");
    vspi_set_start(true);

    gic400_set_priority_mask(&gic, 0xff);
    aarch64_enable_irq();

    assert_14();
    set_priority(0x10);

    gic400_dump_ns_cpuif(&gic);
    gic400_dump_ns_distributor(&gic);

    printk("<%s> Done.\n", __func__);
    vspi_set_start(false);

    __asm volatile("hvc #0");
}

static void vspi_14_primary(void)
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

static void vspi_14_secondary(void)
{
    while (! vspi_get_start()) {
        arm_wfe();
    }

    printk("<%s>\n", __func__);

    assert_14();
    set_priority(0x10);
    vspi_set_start(false);
}

void test_vspi_14(void)
{
    if (cpu_no() != 0) {
        return;
    }

    vspi_14();
}

void test_vspi_14m(void)
{
    if (cpu_no() == 0) {
        vspi_14_primary();
    } else if (cpu_no() == 1) {
        vspi_14_secondary();
    }
}
