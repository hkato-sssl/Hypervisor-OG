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

/* includes */

#include <stdint.h>

/* defines */

/* types */

/* variables */

/* functions */

#ifdef __cplusplus
extern "C" {
#endif

#define REG_WRITE64(b, r, d)   (*(volatile uint64_t *)((b) + (r)) = (uint64_t)(d))
#define REG_WRITE32(b, r, d)   (*(volatile uint32_t *)((b) + (r)) = (uint32_t)(d))
#define REG_WRITE16(b, r, d)   (*(volatile uint16_t *)((b) + (r)) = (uint16_t)(d))
#define REG_WRITE8(b, r, d)    (*(volatile uint8_t *)((b) + (r)) = (uint8_t)(d))

#define REG_READ64(b, r)       (*(volatile uint64_t *)((b) + (r)))
#define REG_READ32(b, r)       (*(volatile uint32_t *)((b) + (r)))
#define REG_READ16(b, r)       (*(volatile uint16_t *)((b) + (r)))
#define REG_READ8(b, r)        (*(volatile uint8_t *)((b) + (r)))

#define MEMORY_BARRIER()        __asm volatile ("dsb osh")

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_SYSTEM_MEMIO_H */

