/*
 * hvc/hvc_set_result.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/vpc.h"
#include "hypervisor/vpc_register.h"
#include "lib/system/errno.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t hvc_set_result(struct vpc *vpc, uint64_t result)
{
    vpc->regs[VPC_X0] = result;

    return SUCCESS;
}
