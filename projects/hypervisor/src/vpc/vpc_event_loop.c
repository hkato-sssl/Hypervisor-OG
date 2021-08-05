/*
 * vpc/vpc_event_loop.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/vpc.h"
#include "lib/system/errno.h"
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
        }
    } while (ret == SUCCESS);

    return ret;
}
