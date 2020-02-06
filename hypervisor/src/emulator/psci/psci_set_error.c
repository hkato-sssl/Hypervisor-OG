/*
 * emulator/psci/psci_set_error.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/psci.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t psci_set_error(struct vpc *vpc, int64_t error)
{
    vpc->regs[VPC_X0] = (uint64_t)error;

    return SUCCESS;
}

