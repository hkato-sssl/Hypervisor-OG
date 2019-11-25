/*
 * lib/system/spin_lock.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef LIB_SYSTEM_SPIN_LOCK_H
#define LIB_SYSTEM_SPIN_LOCK_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>
#include "lib/system/errno.h"

/* defines */

/* types */

typedef uint32_t spin_lock_t;

/* variables */

/* functions */

errno_t spin_lock_init(spin_lock_t *lock);
errno_t spin_trylock(spin_lock_t *lock);
void spin_lock(spin_lock_t *lock);
void spin_unlock(spin_lock_t *lock);

errno_t system_spin_lock_init(spin_lock_t *lock);
void system_spin_lock(void);
void system_spin_unlock(void);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_SYSTEM_SPIN_LOCK_H */

