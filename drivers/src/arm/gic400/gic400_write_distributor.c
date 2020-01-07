/*
 * arm/gic400/gic400_write_distributor.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/memio.h"
#include "driver/arm/gic400.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

void gic400_write_distributor(const struct gic400 *gic, uintptr_t reg, uint32_t d)
{
    REG_WRITE32(gic->config.base.distributor, reg, d);
}

void gic400_write_sync_distributor(const struct gic400 *gic, uintptr_t reg, uint32_t d)
{
    REG_WRITE32_SYNC(gic->config.base.distributor, reg, d);
}

