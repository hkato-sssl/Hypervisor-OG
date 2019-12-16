/*
 * system/system_spin_lock.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"
#include "lib/system.h"

/* defines */

/* types */

/* prototypes */

/* variables */

static spin_lock_t *system_lock;

/* functions */

errno_t system_register_spin_lock(spin_lock_t *lock)
{
    errno_t ret;

    ret = system_validate_stack_region(lock, sizeof(*lock));
    if (ret != SUCCESS) {
        spin_lock_init(lock);
        system_lock = lock;
    }

    return ret;
}

void system_spin_lock(void)
{
    spin_lock(system_lock);
}

void system_spin_unlock(void)
{
    spin_unlock(system_lock);
}
