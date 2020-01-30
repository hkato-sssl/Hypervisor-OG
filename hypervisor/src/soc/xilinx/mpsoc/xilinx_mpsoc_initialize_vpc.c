/*
 * soc/xilinx/mpsoc/xilinx_mpsoc_initialize_vpc.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <assert.h>
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"
#include "hypervisor/soc/xilinx/mpsoc.h"
#include "mpsoc_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t xilinx_mpsoc_initialize_vpc(struct xilinx_mpsoc *chip, struct vpc *vpc, const struct vpc_configuration *config)
{
    errno_t ret;

    ret = soc_initialize_vpc(&(chip->soc), vpc, config);

    return ret;
}

