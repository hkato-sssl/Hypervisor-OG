/*
 * lib/spin_lock.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef LIB_SPIN_LOCK_H
#define LIB_SPIN_LOCK_H

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

typedef int32_t spin_lock_t;

/* variables */

/* functions */

int spin_lock(spin_lock_t *lock);
void spin_unlock(spin_lock_t *lock);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_SPIN_LOCK_H */
