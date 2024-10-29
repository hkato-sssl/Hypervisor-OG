// SPDX-License-Identifier: Apache-2.0
/*
 * vpc/vpc_event_loop.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/vpc.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t vpc_event_loop(struct vpc *vpc)
{
    errno_t ret;

    do {
        ret = vpc_emulate_exception(vpc);
        if (ret == SUCCESS) {
            ret = vpc_resume(vpc);
            if (ret != SUCCESS) {
                printk("vpc_resume() -> %d\n", ret);
            }
        } else {
            printk("vpc_emulate_exception() -> %d\n", ret);
        }
    } while (ret == SUCCESS);

    return ret;
}
