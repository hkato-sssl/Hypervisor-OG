// SPDX-License-Identifier: Apache-2.0
/*
 * driver/aarch64/at.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef DRIVER_AARCH64_AT_H
#define DRIVER_AARCH64_AT_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "lib/system/errno.h"
#include <stdint.h>

/* defines */

/* types */

/* variables */

/* functions */

uint64_t aarch64_at_s1e0r(uint64_t va);
uint64_t aarch64_at_s1e0w(uint64_t va);
uint64_t aarch64_at_s1e1r(uint64_t va);
uint64_t aarch64_at_s1e1w(uint64_t va);
uint64_t aarch64_at_s1e2r(uint64_t va);
uint64_t aarch64_at_s1e2w(uint64_t va);

uint64_t aarch64_at_s12e0r(uint64_t va);
uint64_t aarch64_at_s12e0w(uint64_t va);
uint64_t aarch64_at_s12e1r(uint64_t va);
uint64_t aarch64_at_s12e1w(uint64_t va);

errno_t aarch64_va_to_pa_el2(uint64_t *pa, uint64_t va);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_AARCH64_AT_H */
