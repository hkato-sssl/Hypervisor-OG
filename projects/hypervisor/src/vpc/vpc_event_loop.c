/*
 * vpc/vpc_event_loop.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"

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
        }
    } while (ret == SUCCESS);

    return ret;
}

