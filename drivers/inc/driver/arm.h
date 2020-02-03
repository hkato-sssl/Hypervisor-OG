/*
 * driver/arm.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_ARM_H
#define DRIVER_ARM_H

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

static inline uint8_t arm_cpu_no(void)
{
    return aarch64_cpu_no();
}

static inline void arm_enable_irq(void)
{
    aarch64_enable_irq();
}

static inline void arm_disable_irq(void)
{
    aarch64_disable_irq();
}

static inline void arm_enable_fiq(void)
{
    aarch64_enable_fiq();
}

static inline void arm_disable_fiq(void)
{
    aarch64_disable_fiq();
}

static inline void arm_enable_interrupts(void)
{
    aarch64_enable_interrupts();
}

static inline void arm_disable_interrupts(void)
{
    aarch64_disable_interrupts();
}

static inline uint32_t arm_lock_irq(void)
{
    return aarch64_lock_irq();
}

static inline void arm_unlock_irq(uint32_t lock)
{
    aarch64_unlock_irq(lock);
}

static inline uint32_t arm_lock_fiq(void)
{
    return aarch64_lock_fiq();
}

static inline void arm_unlock_fiq(uint32_t lock)
{
    aarch64_unlock_fiq(lock);
}

static inline uint32_t arm_lock_interrupts(void)
{
    return aarch64_lock_interrupts();
}

static inline void arm_unlock_interrupts(uint32_t lock)
{
    aarch64_unlock_interrupts(lock);
}

static inline void arm_wfe(void)
{
    __asm volatile ("wfe" ::: "memory");
}

static inline void arm_wfi(void)
{
    __asm volatile ("wfi" ::: "memory");
}

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_ARM_H */

