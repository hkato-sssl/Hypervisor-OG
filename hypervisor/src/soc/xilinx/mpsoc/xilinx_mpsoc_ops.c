/*
 * soc/xilinx/mpsoc/xilinx_mpsoc_ops.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "hypervisor/soc.h"
#include "hypervisor/soc/xilinx/mpsoc.h"
#include "mpsoc_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

struct soc_ops xilinx_mpsoc_ops = {
    .test_executable_region = soc_default_test_executable_region,
};

/* functions */

