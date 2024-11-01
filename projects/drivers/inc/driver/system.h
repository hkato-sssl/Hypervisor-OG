// SPDX-License-Identifier: Apache-2.0
/*
 * driver/system.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef DRIVER_SYSTEM_H
#define DRIVER_SYSTEM_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "driver/system/cpu.h"
#include <stdint.h>

/* defines */

/* types */

/* variables */

/* functions */

static inline void udelay(uint32_t usec)
{
    aarch64_udelay(usec);
}

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_SYSTEM_H */
