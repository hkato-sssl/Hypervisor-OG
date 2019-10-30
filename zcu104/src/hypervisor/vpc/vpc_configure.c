/*
 * vpc/vpc_configure.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "lib/system/errno.h"
#include "driver/aarch64/system_register.h"
#include "hypervisor/vpc.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static void setup_aarch64(struct vpc *vpc)
{
    /* SPSR:
     *    I, F = 1 - IRQ and FIQ are masked
     *    M[4:0] = 0x05 - AArch64 EL1h
     */
    vpc->regs[VPC_SPSR_EL2] = PSTATE_I | PSTATE_F | 0x05;
}

static void setup_aarch32(struct vpc *vpc)
{
    /* SPSR:
     *    I, F = 1 - IRQ and FIQ are masked
     *    M[4:0] = 0x13 - AArch32 Supervisor mode
     */
    vpc->regs[VPC_SPSR_EL2] = PSTATE_I | PSTATE_F | 0x13;
}

static errno_t configure(struct vpc *vpc, const struct vpc_config *config)
{
    memset(vpc, 0, sizeof(struct vpc));
    memset(vpc->regs, 0, (sizeof(uint64_t) * NR_VPC_REGS));

    vpc->owner = config->owner;
    vpc->regs = config->regs;

    vpc->regs[VPC_PC] = config->gpr.pc;
    vpc->regs[VPC_SP_EL1] = config->gpr.sp;

    if (config->aarch == 64) {
        setup_aarch64(vpc);
    } else {
        setup_aarch32(vpc);
    }

    return SUCCESS;
}

static bool is_valid_parameter(struct vpc *vpc, const struct vpc_config *config)
{
    bool ret;

    if ((vpc != NULL) && (config != NULL) && (config->owner != NULL) && (config->regs != NULL)) {
        if ((config->aarch == 32) || (config->aarch == 64)) {
            ret = true;
        } else {
            ret = false;
        }
    } else {
        ret = false;
    }

    return ret;
}

errno_t vpc_configure(struct vpc *vpc, const struct vpc_config *config)
{
    errno_t ret;

    if (is_valid_parameter(vpc, config)) {
        ret = configure(vpc, config);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

