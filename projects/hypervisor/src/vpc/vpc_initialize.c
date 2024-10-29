// SPDX-License-Identifier: Apache-2.0
/*
 * vpc/vpc_initialize.c
 *
 * (C) 2019 Hidekazu Kato
 *
 */

#include "driver/aarch64/system_register.h"
#include "hypervisor/vpc.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t initialize(struct vpc *vpc,
                          const struct vpc_configuration *config)
{
    memset(vpc, 0, sizeof(struct vpc));
    memset(config->regs, 0, (sizeof(uint64_t) * NR_VPC_REGS));

    vpc->vm = config->vm;
    vpc->regs = config->regs;
    vpc->proc_no = config->proc_no;
    vpc->hook = config->hook;
    vpc->exception.ops = config->exception.ops;

    return SUCCESS;
}

static errno_t validate_parameters(const struct vpc *vpc,
                                   const struct vpc_configuration *config)
{
    errno_t ret;

    if (vpc == NULL) {
        ret = -EINVAL;
    } else if (config == NULL) {
        ret = -EINVAL;
    } else if (config->vm == NULL) {
        ret = -EINVAL;
    } else if (config->regs == NULL) {
        ret = -EINVAL;
    } else if (! IS_ALIGNED(vpc->regs, 32)) {
        ret = -EINVAL;
    } else if (config->exception.ops == NULL) {
        ret = -EINVAL;
    } else {
        ret = SUCCESS;
    }

    return ret;
}

errno_t vpc_initialize(struct vpc *vpc, const struct vpc_configuration *config)
{
    errno_t ret;

    ret = validate_parameters(vpc, config);
    if (ret == SUCCESS) {
        ret = initialize(vpc, config);
    }

    return ret;
}
