/*
 * lib/bit.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef LIB_BIT_H
#define LIB_BIT_H

#ifdef ASSEMBLY

/* defines */

#define BIT(n)              (1 << (n))
#define BITS(m, l)          ((BIT(m) | (BIT(m) - 1)) ^ (BIT(l) - 1))

#else /* ASSEMBLY */

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>

/* defines */

#define BIT(n)              (1ULL << (n))
#define BITS(m, l)          ((BIT(m) | (BIT(m) - 1)) ^ (BIT(l) - 1))
#define IS_ALIGNED(n, a)    (((uintptr_t)(n) & ((uintptr_t)(a) - 1)) == 0)

/* types */

/* variables */

/* functions */

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_BIT_H */

