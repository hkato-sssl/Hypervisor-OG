// SPDX-License-Identifier: Apache-2.0
/*
 * vpc/vpc_wait_request.c
 *
 * (C) 2020 Hidekazu Kato
 *
 * NOTE:
 * 本APIはvm_ready()から呼び出される事を前提とした実装である。
 */

#include "driver/arm.h"
#include "hypervisor/vpc.h"
#include "lib/system/errno.h"
#include "vpc_local.h"
#include <assert.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t wait_request(struct vpc *vpc)
{
    enum vpc_status status;

    vpc_set_status(vpc, VPC_STATUS_WAIT);

    do {
        arm_wfe();
        status = vpc_watch_status(vpc);
    } while (status != VPC_STATUS_WAKEUP);

    return SUCCESS;
}

errno_t vpc_wait_request(struct vpc *vpc)
{
    errno_t ret;
    enum vpc_status status;

    ret = validate_vpc(vpc);
    if (ret == SUCCESS) {
        status = vpc_watch_status(vpc);
        if (status == VPC_STATUS_DOWN) {
            ret = wait_request(vpc);
        } else {
            ret = -EPERM;
        }
    }

    return ret;
}
