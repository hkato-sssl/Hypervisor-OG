/*
 * driver/arm/gic400/gic400_local.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_ARM_GIC400_GIC400_LOCAL_H
#define DRIVER_ARM_GIC400_GIC400_LOCAL_H

/* includes */

/* defines */

#ifndef ASSEMBLY

/* includes */

#include <stdint.h>
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "driver/arm/gic400.h"

/* defines */

/* types */

/* variables */

/* functions */

#ifdef __cplusplus
extern "C" {
#endif

errno_t gic400_dist_write_bit(struct gic400 const *gic, uint16_t bit_no, uintptr_t reg0);

static inline void gic400_write_cpuif(struct gic400 const *gic, uintptr_t reg, uint32_t d)
{
    REG_WRITE32(gic->config.base.cpuif, reg, d);
}

static inline void gic400_write_sync_cpuif(struct gic400 const *gic, uintptr_t reg, uint32_t d)
{
    REG_WRITE32(gic->config.base.cpuif, reg, d);
    memory_barrier();
    REG_READ32(gic->config.base.cpuif, reg);
}

static inline uint32_t gic400_read_cpuif(struct gic400 const *gic, uintptr_t reg)
{
    uint32_t d;

    d = REG_READ32(gic->config.base.cpuif, reg);

    return d;
}

static inline void gic400_write_dist(struct gic400 const *gic, uintptr_t reg, uint32_t d)
{
    REG_WRITE32(gic->config.base.dist, reg, d);
}

static inline void gic400_write_sync_dist(struct gic400 const *gic, uintptr_t reg, uint32_t d)
{
    REG_WRITE32(gic->config.base.dist, reg, d);
    memory_barrier();
    REG_READ32(gic->config.base.dist, reg);
}

static inline uint32_t gic400_read_dist(struct gic400 const *gic, uintptr_t reg)
{
    uint32_t d;

    d = REG_READ32(gic->config.base.dist, reg);

    return d;
}

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_ARM_GIC400_GIC400_LOCAL_H */

