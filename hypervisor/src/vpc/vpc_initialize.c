/*
 * vpc/vpc_initialize.c
 *
 * (C) 2019 Hidekazu Kato
 *
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "driver/aarch64/system_register.h"
#include "hypervisor/vpc.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t initialize(struct vpc *vpc, const struct vpc_configuration *config)
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

static bool is_valid_parameter(struct vpc *vpc, const struct vpc_configuration *config)
{
    bool ret;

    if ((vpc != NULL) && (config != NULL) && (config->vm != NULL) && (config->regs != NULL) && IS_ALIGNED(vpc->regs, 32) && (config->exception.ops != NULL)) {
        ret = true;
    } else {
        ret = false;
    }

    return ret;
}

errno_t vpc_initialize(struct vpc *vpc, const struct vpc_configuration *config)
{
    errno_t ret;

    if (is_valid_parameter(vpc, config)) {
        ret = initialize(vpc, config);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

