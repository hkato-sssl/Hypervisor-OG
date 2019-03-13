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

#include <stdint.h>
#include "driver/aarch64.h"

/* defines */

/* types */

/* variables */

/* functions */

static inline uint8_t cpu_no(void)
{
    return aarch64_cpu_no();
}

static inline uint32_t cpu_lock_interrupts(void)
{
    return aarch64_lock_interrupts();
}

static inline void cpu_unlock_interrupts(bool lock)
{
    aarch64_unlock_interrupts(lock);
}

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_SYSTEM_CPU_H */

