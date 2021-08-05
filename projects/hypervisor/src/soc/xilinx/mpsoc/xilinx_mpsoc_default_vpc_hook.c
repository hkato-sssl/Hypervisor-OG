/*
 * soc/xilinx/mpsoc/xilinx_mpsoc_default_vpc_hook.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/vpc.h"
#include "mpsoc_local.h"
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

static const struct vpc_hook hook = {
    .launch.previous = xilinx_mpsoc_previous_launch_hook,
    .launch.post = NULL,
    .resume.previous = NULL,
    .resume.post = NULL,
};

/* functions */

const struct vpc_hook *xilinx_mpsoc_default_vpc_hook(void)
{
    return &hook;
}
