/*
 * vpc/vpc_wait_request.c
 *
 * (C) 2020 Hidekazu Kato
 *
 * CAUTION:
 * 本APIはvpc_launch()から呼び出される事を前提とした実装である。
 */

#include <stdint.h>
#include <assert.h>
#include "lib/system/errno.h"
#include "driver/arm.h"
#include "hypervisor/vpc.h"
#include "vpc_local.h"

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

    if (is_valid_vpc(vpc)) {
        status = vpc_watch_status(vpc);
        if (status == VPC_STATUS_DOWN) {
            ret = wait_request(vpc);
        } else {
            ret = -EPERM;
        }
    } else {
        ret = -EINVAL;
    }

    return ret;
}
