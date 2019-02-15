/*
 * libs/atomic.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef LIBS_ATOMIC_H
#define LIBS_ATOMIC_H

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

void atomic_add_u64(uint64_t *p, uint64_t d);
void atomic_sub_u64(uint64_t *p, uint64_t d);

static void atomic_inc_u64(uint64_t *p)
{
    atomic_add_u64(p, 1);
}

static void atomic_dec_u64(uint64_t *p)
{
    atomic_sub_u64(p, 1);
}

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIBS_ATOMIC_H */

