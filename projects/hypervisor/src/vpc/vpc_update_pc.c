/*
 * vpc/vpc_update_pc.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/aarch64/system_register/esr_elx.h"
#include "hypervisor/vpc.h"
#include "lib/system/errno.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t vpc_update_pc(struct vpc *vpc)
{
    uint64_t d;

    d = vpc->regs[VPC_ESR_EL2];
    if ((d & ESR_IL) == 0) {
        vpc->regs[VPC_PC] += 2; /* 16-bit instruction */
    } else {
        vpc->regs[VPC_PC] += 4; /* 32-bit instruction */
    }

    return SUCCESS;
}
