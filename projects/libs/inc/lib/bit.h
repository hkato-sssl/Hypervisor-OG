/*
 * lib/bit.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef LIB_BIT_H
#define LIB_BIT_H

#define ALIGN(v, a) ((((v) + ((a)-1)) | ((a)-1)) ^ ((a)-1))

#ifdef ASSEMBLY

/* defines */

#define BIT(n)            (1 << (n))
#define BITS(m, l)        ((BIT(m) | (BIT(m) - 1)) ^ (BIT(l) - 1))
#define BF_VALUE(d, m, l) (BITS((m), (l)) & ((d) << (l)))

#else /* ASSEMBLY */

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdbool.h>
#include <stdint.h>

/* defines */

#define BIT(n)           (1ULL << (n))
#define BITS(m, l)       ((BIT(m) | (BIT(m) - 1)) ^ (BIT(l) - 1))
#define IS_ALIGNED(n, a) lib_is_aligned((uintptr_t)(n), (uintptr_t)(a))
#define BF_EXTRACT(d, m, l) \
    lib_bf_extract((uintptr_t)(d), (uintptr_t)(m), (uintptr_t)(l))
#define BF_VALUE(d, m, l) (BITS((m), (l)) & ((d) << (l)))

/* types */

/* variables */

/* functions */

static inline bool lib_is_aligned(uintptr_t d, uintptr_t a)
{
    bool ret;

    if ((d & (a - 1)) == 0) {
        ret = true;
    } else {
        ret = false;
    }

    return ret;
}

static inline uintptr_t lib_bf_extract(uintptr_t d, uintptr_t msb,
                                       uintptr_t lsb)
{
    uintptr_t bf;

    bf = (d >> lsb) & BITS((msb - lsb), 0);

    return bf;
}

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_BIT_H */
