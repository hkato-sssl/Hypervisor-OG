// SPDX-License-Identifier: Apache-2.0
/*
 * system/system_spin_lock.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/system.h"
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

static spin_lock_t *system_lock;

/* functions */

errno_t system_register_spin_lock(spin_lock_t *lock)
{
    errno_t ret;

    if (! system_test_valid_stack_region((void *)lock, sizeof(*lock))) {
        spin_lock_init(lock);
        system_lock = lock;
        ret = SUCCESS;
    } else {
        ret = -EFAULT;
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
