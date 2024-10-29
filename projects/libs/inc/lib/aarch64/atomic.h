// SPDX-License-Identifier: Apache-2.0
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

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>

/* defines */

/* types */

/* variables */

/* functions */

void aarch64_atomic_add_u64(uint64_t *p, uint64_t d);
void aarch64_atomic_sub_u64(uint64_t *p, uint64_t d);
uint8_t aarch64_atomic_swap_u8(uint8_t *p, uint8_t d);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_AARCH64_ATOMIC_H */
