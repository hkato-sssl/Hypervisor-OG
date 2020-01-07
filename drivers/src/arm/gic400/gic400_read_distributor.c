/*
 * arm/gic400/gic400_read_distributor.c
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

uint32_t gic400_read_distributor(const struct gic400 *gic, uintptr_t reg)
{
    uint32_t d;

    d = REG_READ32(gic->config.base.distributor, reg);

    return d;
}

