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

static errno_t init_system_register(struct vpc *vpc, const struct vpc_config *config)
{
    uint64_t d;

    vpc->regs[VPC_SCTLR_EL1] = DEFAULT_SCTLR_EL1;

    d = aarch64_read_midr_el1();
    vpc->regs[VPC_VPIDR_EL2] = d;
    vpc->regs[VPC_VMPIDR_EL2] = BIT(31) | config->proc_no;

    return SUCCESS;
}

static errno_t setup_aarch64(struct vpc *vpc, const struct vpc_config *config)
{
    errno_t ret;

    /* SPSR:
     *    D, A, I, F = 1
     *    M[4:0] = 0x05 - AArch64 EL1h
     */
    vpc->regs[VPC_SPSR_EL2] = PSTATE_D | PSTATE_A | PSTATE_I | PSTATE_F | 0x05;
    vpc->regs[VPC_HCR_EL2] = BIT(31);	// HCR_EL2.RW = 1

    ret = init_system_register(vpc, config);

    return ret;
}

static errno_t setup_aarch32(struct vpc *vpc, const struct vpc_config *config)
{
    errno_t ret;

    /* SPSR:
     *    I, F = 1 - IRQ and FIQ are masked
     *    M[4:0] = 0x13 - AArch32 Supervisor mode
     */
    vpc->regs[VPC_SPSR_EL2] = PSTATE_I | PSTATE_F | 0x13;

    ret = init_system_register(vpc, config);

    return ret;
}

static errno_t configure(struct vpc *vpc, const struct vpc_config *config)
{
    errno_t ret;

    memset(vpc, 0, sizeof(struct vpc));
    memset(vpc->regs, 0, (sizeof(uint64_t) * NR_VPC_REGS));

    vpc->owner = config->owner;
    vpc->regs = config->regs;

    vpc->regs[VPC_PC] = config->gpr.pc;
    vpc->regs[VPC_SP_EL1] = config->gpr.sp;

    if (config->aarch == 64) {
        ret = setup_aarch64(vpc, config);
    } else {
        ret = setup_aarch32(vpc, config);
    }

    return ret;
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

    // VPCを起動する為に最低限の設定を行う

    if (is_valid_parameter(vpc, config)) {
        ret = configure(vpc, config);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

