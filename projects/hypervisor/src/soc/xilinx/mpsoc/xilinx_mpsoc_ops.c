// SPDX-License-Identifier: Apache-2.0
/*
 * soc/xilinx/mpsoc/xilinx_mpsoc_ops.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/soc.h"
#include "hypervisor/soc/xilinx/mpsoc.h"
#include "lib/system/errno.h"
#include "mpsoc_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

struct soc_ops xilinx_mpsoc_ops = {
    .test_executable_region = soc_default_test_executable_region,
    .assert_interrupt_vpc = xilinx_mpsoc_assert_interrupt_vpc,
};

/* functions */
