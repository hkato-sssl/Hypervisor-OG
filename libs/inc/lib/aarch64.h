/*
 * lib/aarch64.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef LIB_AARCH64_H
#define LIB_AARCH64_H

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

static inline uint64_t aarch64_cls(uint64_t d)
{
    uint64_t ct;

    __asm volatile ("cls %0, %1" : "=r"(ct) : "r"(d));

    return ct;
}

static inline uint64_t aarch64_clz(uint64_t d)
{
    uint64_t ct;

    __asm volatile ("clz %0, %1" : "=r"(ct) : "r"(d));

    return ct;
}

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_AARCH64_H */

