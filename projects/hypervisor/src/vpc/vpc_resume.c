// SPDX-License-Identifier: Apache-2.0
/*
 * vpc/vpc_resume.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "hypervisor/vpc.h"
#include "vpc_local.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t call_previous_hook(struct vpc *vpc)
{
    errno_t ret;

    if ((vpc->hook != NULL) && (vpc->hook->resume.previous != NULL)) {
        ret = (*(vpc->hook->resume.previous))(vpc);
    } else {
        ret = SUCCESS;
    }

    return ret;
}

static errno_t call_post_hook(struct vpc *vpc)
{
    errno_t ret;

    if ((vpc->hook != NULL) && (vpc->hook->resume.post != NULL)) {
        ret = (*(vpc->hook->resume.post))(vpc);
    } else {
        ret = SUCCESS;
    }

    return ret;
}

static errno_t resume(struct vpc *vpc)
{
    errno_t ret;
    errno_t post;

    ret = call_previous_hook(vpc);
    if (ret == SUCCESS) {
        ret = vpc_switch_to_el1(vpc->regs);
        post = call_post_hook(vpc);
        if ((ret == SUCCESS) && (post != SUCCESS)) {
            ret = post;
        }
    }

    return ret;
}

errno_t vpc_resume(struct vpc *vpc)
{
    errno_t ret;
    enum vpc_status status;

    assert(vpc != NULL);

    status = vpc_watch_status(vpc);
    if (status == VPC_STATUS_RUN) {
        ret = resume(vpc);
    } else {
        ret = -EPERM;
    }

    return ret;
}
