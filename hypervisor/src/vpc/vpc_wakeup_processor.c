/*
 * vpc/vpc_wakeup_processor.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "lib/system/errno.h"
#include "driver/arm.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "vpc_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t wakeup_processor(struct vpc *vpc, const struct vpc_boot_configuration *boot)
{
    vpc_set_boot_parameters(vpc, boot);
    vpc_set_status(vpc, VPC_STATUS_WAKEUP);
    arm_sev();

    return SUCCESS;
}

static errno_t validate_parameters(const struct vpc *vpc, const struct vpc_boot_configuration *boot)
{
    errno_t ret;

    if ((vpc->vm->nr_procs >= vpc->proc_no) || (vpc->vm->vpcs[vpc->proc_no] != vpc)) {
        ret = -EPERM;
    } else if (boot->arch != VPC_ARCH_AARCH64) {
        ret = -EINVAL;
    } else {
        ret = SUCCESS;
    }

    return ret;
}

errno_t vpc_wakeup_processor(struct vpc *vpc, const struct vpc_boot_configuration *boot)
{
    errno_t ret;
    enum vpc_status status;

    assert((vpc != NULL) && (boot != NULL));

    ret = validate_parameters(vpc, boot);
    if (ret == SUCCESS) {
        vpc_lock(vpc);
        status = vpc_watch_status(vpc);
        if (status == VPC_STATUS_WAIT) {
            ret = wakeup_processor(vpc, boot);
        } else {
            ret = -EPERM;
        }
        vpc_unlock(vpc);
    }

    return ret;
}
