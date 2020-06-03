/*
 * soc/xilinx/mpsoc/xilinx_mpsoc_emulate_tz.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"
#include "mpsoc_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t xilinx_mpsoc_emulate_tz(struct vpc *vpc)
{
    errno_t ret;

    ret = xilinx_mpsoc_emulate_tz_asm(vpc->regs);
    if (ret == SUCCESS) {
        ret = vpc_update_pc(vpc);
    }

    return ret;
}
