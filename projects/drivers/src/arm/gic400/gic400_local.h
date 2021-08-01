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

errno_t gic400_distributor_write_bit(const struct gic400 *gic, uint16_t bit_no, uintptr_t reg0);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_ARM_GIC400_GIC400_LOCAL_H */

