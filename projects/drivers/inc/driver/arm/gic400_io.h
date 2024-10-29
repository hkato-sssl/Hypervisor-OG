// SPDX-License-Identifier: Apache-2.0
/*
 * driver/arm/gic400_io.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef DRIVER_ARM_GIC400_IO_H
#define DRIVER_ARM_GIC400_IO_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "driver/arm/gic400.h"
#include "lib/system/memio.h"
#include <stdint.h>

/* defines */

/* types */

/* variables */

/* inline functions */

static inline void gic400_lock(struct gic400 *gic)
{
    spin_lock(&(gic->lock));
}

static inline void gic400_unlock(struct gic400 *gic)
{
    spin_unlock(&(gic->lock));
}

static inline uint8_t gic400_priority_mask(const struct gic400 *gic)
{
    return (0xff << gic->priority.shift_ct);
}

static inline void *gic400_cpuif_register_base(const struct gic400 *gic)
{
    return gic->configuration.base.cpuif;
}

static inline void *gic400_distributor_register_base(const struct gic400 *gic)
{
    return gic->configuration.base.distributor;
}

static inline void gic400_write_cpuif(const struct gic400 *gic, uintptr_t reg,
                                      uint32_t d)
{
    REG_WRITE32(gic->configuration.base.cpuif, reg, d);
}

static inline uint32_t gic400_read_cpuif(const struct gic400 *gic,
                                         uintptr_t reg)
{
    uint32_t d;

    d = REG_READ32(gic->configuration.base.cpuif, reg);

    return d;
}

static inline void gic400_write_sync_cpuif(const struct gic400 *gic,
                                           uintptr_t reg, uint32_t d)
{
    REG_WRITE32_SYNC(gic->configuration.base.cpuif, reg, d);
}

static inline void gic400_write_distributor_b(const struct gic400 *gic,
                                              uintptr_t reg, uint32_t d)
{
    REG_WRITE8(gic->configuration.base.distributor, reg, d);
}

static inline void gic400_write_distributor(const struct gic400 *gic,
                                            uintptr_t reg, uint32_t d)
{
    REG_WRITE32(gic->configuration.base.distributor, reg, d);
}

static inline void gic400_write_sync_distributor(const struct gic400 *gic,
                                                 uintptr_t reg, uint32_t d)
{
    REG_WRITE32_SYNC(gic->configuration.base.distributor, reg, d);
}

static inline uint32_t gic400_read_distributor_b(const struct gic400 *gic,
                                                 uintptr_t reg)
{
    uint32_t d;

    d = REG_READ8(gic->configuration.base.distributor, reg);

    return d;
}

static inline uint32_t gic400_read_distributor(const struct gic400 *gic,
                                               uintptr_t reg)
{
    uint32_t d;

    d = REG_READ32(gic->configuration.base.distributor, reg);

    return d;
}

/* functions */

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_ARM_GIC400_IO_H */
