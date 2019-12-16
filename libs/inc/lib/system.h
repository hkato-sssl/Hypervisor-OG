/*
 * lib/system.h
 *
 * (C) 2018 Hidekazu Kato
 */

#ifndef LIB_SYSTEM_H
#define LIB_SYSTEM_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"

/* defines */

/* variables */

/* functions */

errno_t system_register_spin_lock(spin_lock_t *lock);
void system_spin_lock(void);
void system_spin_unlock(void);
errno_t system_validate_stack_region(void *addr, size_t size);

void udelay(uint32_t usec);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_SYSTEM_H */

