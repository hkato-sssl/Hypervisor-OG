/*
 * driver/aarch64.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_AARCH64_H
#define DRIVER_AARCH64_H

/* includes */

/* defines */

#ifndef ASSEMBLY

/* includes */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* defines */

/* types */

/* variables */

/* functions */

uint8_t aarch64_cpu_no(void);

uint32_t aarch64_lock_irq(void);
void aarch64_unlock_irq(uint32_t lock);
uint32_t aarch64_lock_fiq(void);
void aarch64_unlock_fiq(uint32_t lock);
uint32_t aarch64_lock_interrupts(void);
void aarch64_unlock_interrupts(uint32_t lock);

static inline void aarch64_isb(void)
{
    __asm volatile ("isb" ::: "memory");
}

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_AARCH64_H */

