/*
 * test/arm/gic400/01.c
 *
 * (C) 2019 Hidekazu Kato
 */

/* テスト項目：初期化処理
 *
 * 初期化APIを実行後、レジスタ情報を出力して実行結果を確認する。
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "lib/system/printk.h"
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

errno_t test_arm_gic400_01(void)
{
    errno_t ret;

    printk("<%s>\n", __func__);
    ret = init();
    if (ret != SUCCESS) {
        return ret;
    }

    gic400_dump_ns_cpuif(&gic);
    gic400_dump_ns_distributor(&gic);

    printk("Done\n");

    return SUCCESS;
}


