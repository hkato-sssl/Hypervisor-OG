// SPDX-License-Identifier: Apache-2.0
/*
 * driver/system/cpu.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_SYSTEM_CPU_H
#define DRIVER_SYSTEM_CPU_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "driver/aarch64.h"
#include <stdbool.h>
#include <stdint.h>

/* defines */

/* types */

/* variables */

/* functions */

static inline uint32_t cpu_no(void)
{
    return aarch64_cpu_no();
}

static inline uint32_t cpu_lock_interrupts(void)
{
    return aarch64_lock_interrupts();
}

static inline void cpu_unlock_interrupts(uint32_t lock)
{
    aarch64_unlock_interrupts(lock);
}

void cpu_disable_interrupts(void);
void cpu_enable_interrupts(void);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_SYSTEM_CPU_H */
