// SPDX-License-Identifier: Apache-2.0
/*
 * soc/xilinx/mpsoc/xilinx_mpsoc_default_vpc_exception_ops.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/soc/xilinx/mpsoc.h"
#include "hypervisor/vpc.h"
#include "mpsoc_local.h"
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

static const struct vpc_exception_ops ops = {
    .irq = xilinx_mpsoc_emulate_irq_exception,
    .aarch64.data_abort = vpc_emulate_aarch64_data_abort,
    .aarch64.hvc = xilinx_mpsoc_emulate_hvc,
    .aarch64.smc = xilinx_mpsoc_emulate_smc,
};

/* functions */

const struct vpc_exception_ops *xilinx_mpsoc_default_vpc_exception_ops(void)
{
    return &ops;
}
