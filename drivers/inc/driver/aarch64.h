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

void aarch64_enable_irq(void);
void aarch64_disable_irq(void);
void aarch64_enable_fiq(void);
void aarch64_disable_fiq(void);
void aarch64_enable_interrupts(void);
void aarch64_disable_interrupts(void);

uint32_t aarch64_lock_irq(void);
void aarch64_unlock_irq(uint32_t lock);
uint32_t aarch64_lock_fiq(void);
void aarch64_unlock_fiq(uint32_t lock);
uint32_t aarch64_lock_interrupts(void);
void aarch64_unlock_interrupts(uint32_t lock);

void aarch64_udelay(uint32_t usec);

static inline void aarch64_isb(void)
{
    __asm volatile ("isb" ::: "memory");
}

static inline void aarch64_dmb(void)
{
    __asm volatile ("dmb sy" ::: "memory");
}

static inline void aarch64_dmb_osh(void)
{
    __asm volatile ("dmb osh" ::: "memory");
}

static inline void aarch64_dmb_ish(void)
{
    __asm volatile ("dmb ish" ::: "memory");
}

static inline void aarch64_dsb(void)
{
    __asm volatile ("dsb sy" ::: "memory");
}

static inline void aarch64_dsb_osh(void)
{
    __asm volatile ("dsb osh" ::: "memory");
}

static inline void aarch64_dsb_ish(void)
{
    __asm volatile ("dsb ish" ::: "memory");
}

static inline void aarch64_sev(void)
{
    __asm volatile ("sev" ::: "memory");
}

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_AARCH64_H */

