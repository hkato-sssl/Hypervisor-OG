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

/* types */

/* variables */

/* functions */

#define REG_WRITE64(b, r, d)   (*(volatile uint64_t *)((uintptr_t)(b) + (uintptr_t)(r)) = (uint64_t)(d))
#define REG_WRITE32(b, r, d)   (*(volatile uint32_t *)((uintptr_t)(b) + (uintptr_t)(r)) = (uint32_t)(d))
#define REG_WRITE16(b, r, d)   (*(volatile uint16_t *)((uintptr_t)(b) + (uintptr_t)(r)) = (uint16_t)(d))
#define REG_WRITE8(b, r, d)    (*(volatile uint8_t *)((uintptr_t)(b) + (uintptr_t)(r)) = (uint8_t)(d))

#define REG_READ64(b, r)       (*(volatile uint64_t *)((uintptr_t)(b) + (uintptr_t)(r)))
#define REG_READ32(b, r)       (*(volatile uint32_t *)((uintptr_t)(b) + (uintptr_t)(r)))
#define REG_READ16(b, r)       (*(volatile uint16_t *)((uintptr_t)(b) + (uintptr_t)(r)))
#define REG_READ8(b, r)        (*(volatile uint8_t *)((uintptr_t)(b) + (uintptr_t)(r)))

#define REG_WRITE64_SYNC(b, r, d)   (REG_WRITE64((b), (r), (d)), REG_READ64((b), (r)))
#define REG_WRITE32_SYNC(b, r, d)   (REG_WRITE32((b), (r), (d)), REG_READ32((b), (r)))
#define REG_WRITE16_SYNC(b, r, d)   (REG_WRITE16((b), (r), (d)), REG_READ16((b), (r)))
#define REG_WRITE8_SYNC(b, r, d)    (REG_WRITE8((b), (r), (d)), REG_READ8((b), (r)))

static inline void memory_barrier(void)
{
	__asm volatile ("dsb osh" ::: "memory");
}

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_SYSTEM_MEMIO_H */

