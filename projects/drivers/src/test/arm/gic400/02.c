/*
 * test/arm/gic400/02.c
 *
 * (C) 2019 Hidekazu Kato
 */

/* テスト項目：SGI通知(Self)
 *
 * 自コアに対してSGI#0-15の割り込みを発生させる。
 * 全割り込み通知を認識する事を確認。
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "driver/arm.h"
#include "driver/arm/gic400.h"

/* defines */

/* test parameters */

#define GIC400_DIST_BASE    0xf9010000
#define GIC400_CPUIF_BASE   0xf9020000

/* types */

/* prototypes */

/* variables */

static struct gic400 gic;

/* functions */

static errno_t init(void)
{
    errno_t ret;
    struct gic400_configuration config;

    memset(&config, 0, sizeof(config));
    config.base.distributor = (void *)GIC400_DIST_BASE;
    config.base.cpuif = (void *)GIC400_CPUIF_BASE;
    ret = gic400_initialize(&gic, &config);
    printk("gic400_init() -> %d\n", ret);

    return ret;
}

errno_t test_arm_gic400_02(void)
{
    errno_t ret;
    struct gic400_interrupt_configuration config;

    printk("<%s>\n", __func__);
    ret = init();
    if (ret != SUCCESS) {
        return ret;
    }

    gic400_dump_ns_cpuif(&gic);
    gic400_dump_ns_distributor(&gic);
    printk("----------\n");

    arm_disable_interrupts();

    memset(&config, 0, sizeof(config));
    config.targets = 1;
    config.priority = 1;
    ret = gic400_configure_interrupt(&gic, 7, &config);
    printk("gic400_configure_interrupt() -> %d\n", ret);

    ret = gic400_assert_sgi(&gic, 1, 7);
    printk("gic400_assert_sgi() -> %d\n", ret);
    ret = gic400_enable_interrupt(&gic, 7);
    printk("gic400_enable_interrupt() -> %d\n", ret);
    ret = gic400_set_priority_mask(&gic, 0xff);
    printk("gic400_set_level() -> %d\n", ret);

    arm_enable_interrupts();

    gic400_dump_ns_cpuif(&gic);
    gic400_dump_ns_distributor(&gic);

    printk("Done\n");

    return SUCCESS;
}


