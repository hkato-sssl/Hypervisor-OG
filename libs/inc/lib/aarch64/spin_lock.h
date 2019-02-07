/*
 * lib/aarch64/spin_lock.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef LIB_AARCH64_SPIN_LOCK_H
#define LIB_AARCH64_SPIN_LOCK_H

/* includes */

#include "lib/spin_lock.h"

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* defines */

/* types */

/* variables */

/* functions */

int aarch64_spin_lock(spin_lock_t *lock);
void aarch64_spin_unlock(spin_lock_t *lock);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_AARCH64_SPIN_LOCK_H */

