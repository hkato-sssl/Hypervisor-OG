/*
 * lib/system/atomic.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef LIB_SYSTEM_ATOMIC_H
#define LIB_SYSTEM_ATOMIC_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>
#include "lib/aarch64/atomic.h"

/* defines */

/* types */

/* variables */

/* functions */

static inline void atomic_add_u64(uint64_t *p, uint64_t d)
{
    aarch64_atomic_add_u64(p, d);
}

static inline void atomic_sub_u64(uint64_t *p, uint64_t d)
{
    aarch64_atomic_sub_u64(p, d);
}

static inline void atomic_inc_u64(uint64_t *p)
{
    atomic_add_u64(p, 1);
}

static inline void atomic_dec_u64(uint64_t *p)
{
    atomic_sub_u64(p, 1);
}

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_SYSTEM_ATOMIC_H */

