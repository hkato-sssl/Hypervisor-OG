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

static errno_t call_post_hook(struct vpc *vpc)
{
    errno_t ret;

    if ((vpc->hook != NULL) && (vpc->hook->post.resume != NULL)) {
        ret = (*(vpc->hook->post.resume))(vpc);
    } else {
        ret = SUCCESS;
    }

    return ret;
}

static errno_t resume(struct vpc *vpc)
{
    errno_t ret;

    ret = vpc_switch_to_el1(vpc->regs);
    if (ret == SUCCESS) {
        ret = call_post_hook(vpc);
    } else {
        /* In this case, ignore the return value. */
        call_post_hook(vpc);
    }

    return ret;
}

static errno_t call_previous_hook(struct vpc *vpc)
{
    errno_t ret;

    if ((vpc->hook != NULL) && (vpc->hook->previous.resume != NULL)) {
        ret = (*(vpc->hook->previous.resume))(vpc);
        if (ret == SUCCESS) {
            ret = resume(vpc);
        }
    } else {
        ret = resume(vpc);
    }

    return ret;
}

errno_t vpc_resume(struct vpc *vpc)
{
    errno_t ret;

    assert(vpc != NULL);

    if (vpc->status == VPC_STATUS_RUNNING) {
        ret = call_previous_hook(vpc);
    } else {
        ret = -EPERM;
    }

    return ret;
}

