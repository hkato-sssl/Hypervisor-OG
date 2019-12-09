/*
 * vpc/vpc_configure.c
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

#define	DEFAULT_SCTLR_EL1   (SCTLR_nTWE | SCTLR_nTWI | SCTLR_CP15BEN | SCTLR_SA0 | SCTLR_SA)

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t configure(struct vpc *vpc, const struct vpc_configuration *config)
{
    memset(vpc, 0, sizeof(struct vpc));
    memset(config->regs, 0, (sizeof(uint64_t) * NR_VPC_REGS));

    vpc->owner = config->owner;
    vpc->regs = config->regs;
    vpc->proc_no = config->proc_no;

    return SUCCESS;
}

static bool is_valid_parameter(struct vpc *vpc, const struct vpc_configuration *config)
{
    bool ret;

    if ((vpc != NULL) && (config != NULL) && (config->owner != NULL) && (config->regs != NULL)) {
        if ((config->arch == VPC_ARCH_AARCH32) || (config->arch == VPC_ARCH_AARCH64)) {
            ret = true;
        } else {
            ret = false;
        }
    } else {
        ret = false;
    }

    return ret;
}

errno_t vpc_configure(struct vpc *vpc, const struct vpc_configuration *config)
{
    errno_t ret;

    // VPCを起動する為に最低限の設定を行う

    if (is_valid_parameter(vpc, config)) {
        ret = configure(vpc, config);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

