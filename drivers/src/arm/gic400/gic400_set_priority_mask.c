/*
 * arm/gic400/gic400_set_priority_mask.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/arm/device/gic400.h"
#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
#include "gic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t set_priority_mask(struct gic400 *gic, uint32_t level)
{
    uint32_t d;

    d = level << gic->priority.shift_ct;
    gic400_write_sync_cpuif(gic, GICC_PMR, d);

    return SUCCESS;
}

errno_t gic400_set_priority_mask(struct gic400 *gic, uint32_t priority)
{
    errno_t ret;

    // GIC-400の仕様としてGICC_PMRに0xffを書き込む事で最低優先度が
    // 設定される事が保証される為、パラメータ値のチェックは省略する。

    if (gic != NULL) {
        ret = set_priority_mask(gic, priority);
    } else {
        ret = -EINVAL;
    }

    return ret;
}
