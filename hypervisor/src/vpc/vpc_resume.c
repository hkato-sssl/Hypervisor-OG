/*
 * vpc/vpc_resume.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "hypervisor/vpc.h"
#include "vpc_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t vpc_resume(struct vpc *vpc)
{
    errno_t ret;

    assert(vpc != NULL);

    if (vpc->boolean.launched) {
        ret = vpc_switch_to_el1(vpc->regs);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

