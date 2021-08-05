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

#include "lib/aarch64/spin_lock.h"

/* defines */

/* types */

/* variables */

/* functions */

errno_t system_spin_lock_init(spin_lock_t *lock);
void system_spin_lock(void);
void system_spin_unlock(void);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_SYSTEM_SPIN_LOCK_H */
