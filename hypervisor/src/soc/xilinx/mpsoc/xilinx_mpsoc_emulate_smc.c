/*
 * soc/xilinx/mpsoc/xilinx_mpsoc_emulate_smc.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"
#include "hypervisor/soc/xilinx/mpsoc.h"
#include "mpsoc_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t xilinx_mpsoc_emulate_smc(struct vpc *vpc)
{
    errno_t ret;
    uint64_t func;

    func = (vpc->regs[VPC_X0] >> 24) & 0xff;
    switch (func) {
    case 0x84:  /* PSCI 32-bit calls */
    case 0xc4:  /* PSCI 64-bit calls */
        ret = xilinx_mpsoc_emulate_psci(vpc);
        break;
    case 0x82:  /* SiP service 32-bit calls */
    case 0xc2:  /* SiP service 64-bit calls */
        ret = xilinx_mpsoc_emulate_tz(vpc);
        break;
    default:
        ret = -ENOTSUP;
        break;
    }

    return ret;
}

