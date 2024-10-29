// SPDX-License-Identifier: Apache-2.0
/*
 * lib/system/memio.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef LIB_SYSTEM_MEMIO_H
#define LIB_SYSTEM_MEMIO_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>

/* defines */

#define REG_READ64(b, r) reg_read64((uintptr_t)(b), (uintptr_t)(r))
#define REG_READ32(b, r) reg_read32((uintptr_t)(b), (uintptr_t)(r))
#define REG_READ16(b, r) reg_read16((uintptr_t)(b), (uintptr_t)(r))
#define REG_READ8(b, r)  reg_read8((uintptr_t)(b), (uintptr_t)(r))

#define REG_WRITE64(b, r, d) \
    reg_write64((uintptr_t)(b), (uintptr_t)(r), (uint64_t)(d))
#define REG_WRITE32(b, r, d) \
    reg_write32((uintptr_t)(b), (uintptr_t)(r), (uint32_t)(d))
#define REG_WRITE16(b, r, d) \
    reg_write16((uintptr_t)(b), (uintptr_t)(r), (uint16_t)(d))
#define REG_WRITE8(b, r, d) \
    reg_write8((uintptr_t)(b), (uintptr_t)(r), (uint8_t)(d))

#define REG_WRITE64_SYNC(b, r, d) \
    reg_write64_sync((uintptr_t)(b), (uintptr_t)(r), (uint64_t)(d))
#define REG_WRITE32_SYNC(b, r, d) \
    reg_write32_sync((uintptr_t)(b), (uintptr_t)(r), (uint32_t)(d))
#define REG_WRITE16_SYNC(b, r, d) \
    reg_write16_sync((uintptr_t)(b), (uintptr_t)(r), (uint16_t)(d))
#define REG_WRITE8_SYNC(b, r, d) \
    reg_write8_sync((uintptr_t)(b), (uintptr_t)(r), (uint8_t)(d))

/* types */

/* variables */

/* inline functions */

static inline void memory_barrier(void)
{
    __asm volatile("dmb sy" ::: "memory");
}

static inline void memory_barrier_sync(void)
{
    __asm volatile("dsb sy" ::: "memory");
}

static inline uint64_t reg_read64(uintptr_t base, uintptr_t reg)
{
    memory_barrier();
    return *(volatile uint64_t *)(base + reg);
}

static inline uint64_t reg_read32(uintptr_t base, uintptr_t reg)
{
    memory_barrier();
    return *(volatile uint32_t *)(base + reg);
}

static inline uint64_t reg_read16(uintptr_t base, uintptr_t reg)
{
    memory_barrier();
    return *(volatile uint16_t *)(base + reg);
}

static inline uint64_t reg_read8(uintptr_t base, uintptr_t reg)
{
    memory_barrier();
    return *(volatile uint8_t *)(base + reg);
}

static inline void reg_write64(uintptr_t base, uintptr_t reg, uint64_t d)
{
    memory_barrier();
    *(volatile uint64_t *)(base + reg) = d;
}

static inline void reg_write32(uintptr_t base, uintptr_t reg, uint32_t d)
{
    memory_barrier();
    *(volatile uint32_t *)(base + reg) = d;
}

static inline void reg_write16(uintptr_t base, uintptr_t reg, uint16_t d)
{
    memory_barrier();
    *(volatile uint16_t *)(base + reg) = d;
}

static inline void reg_write8(uintptr_t base, uintptr_t reg, uint8_t d)
{
    memory_barrier();
    *(volatile uint8_t *)(base + reg) = d;
}

/* functions */

void reg_write64_sync(uintptr_t base, uintptr_t reg, uint64_t d);
void reg_write32_sync(uintptr_t base, uintptr_t reg, uint32_t d);
void reg_write16_sync(uintptr_t base, uintptr_t reg, uint16_t d);
void reg_write8_sync(uintptr_t base, uintptr_t reg, uint8_t d);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_SYSTEM_MEMIO_H */
