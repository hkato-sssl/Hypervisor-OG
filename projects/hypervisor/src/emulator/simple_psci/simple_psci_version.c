/*
 * emulator/simple_psci/simple_psci_version.c
 *
 * (C) 2024 Hidekazu Kato
 */

#include "hypervisor/emulator/simple_psci.h"
#include "hypervisor/vpc.h"
#include "lib/system/errno.h"
#include <stdint.h>

/* defines */

#define EMULATE_VERSION 0x0002 /* 0.2 */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t simple_psci_version(struct vpc *vpc)
{
    vpc->regs[VPC_X0] = EMULATE_VERSION;

    return SUCCESS;
}