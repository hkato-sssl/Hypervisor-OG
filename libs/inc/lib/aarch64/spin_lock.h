/*
 * lib/aarch64/spin_lock.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef LIB_AARCH64_SPIN_LOCK_H
#define LIB_AARCH64_SPIN_LOCK_H

/* includes */

#include <stdint.h>
#include "lib/system/errno.h"

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* defines */

/* types */

typedef uint32_t __attribute__((aligned(32))) spin_lock_t;

/* variables */

/* functions */

errno_t aarch64_spin_lock_init(spin_lock_t *lock);
errno_t aarch64_spin_trylock(spin_lock_t *lock);
void aarch64_spin_lock(spin_lock_t *lock);
void aarch64_spin_unlock(spin_lock_t *lock);

/* inline functions */

static inline errno_t spin_lock_init(spin_lock_t *lock)
{
    return aarch64_spin_lock_init(lock);
}

static inline errno_t spin_trylock(spin_lock_t *lock)
{
    return aarch64_spin_trylock(lock);
}

static inline void spin_lock(spin_lock_t *lock)
{
    aarch64_spin_lock(lock);
}

static inline void spin_unlock(spin_lock_t *lock)
{
    aarch64_spin_unlock(lock);
}

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_AARCH64_SPIN_LOCK_H */

