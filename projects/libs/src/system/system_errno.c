// SPDX-License-Identifier: Apache-2.0
/*
 * system/system_errno.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t system_errno(errno_t code, const char *file, unsigned long line)
{
    printk("ERROR(%d) has occured at %s#%lu.\n", code, file, line);

    return code;
}
