/*
 * lib/aarch64/atomic.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef LIB_AARCH64_ATOMIC_H
#define LIB_AARCH64_ATOMIC_H

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

void aarch64_atomic_add_u64(uint64_t *p, uint64_t d);
void aarch64_atomic_sub_u64(uint64_t *p, uint64_t d);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_AARCH64_ATOMIC_H */

