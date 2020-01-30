/*
 * soc/xilinx/mpsoc/xilinx_mpsoc_default_vpc_hook.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "hypervisor/vpc.h"
#include "mpsoc_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

static const struct vpc_hook hook = {
    .previous.launch = xilinx_mpsoc_previous_launch_hook,
    .previous.resume = NULL,
    .post.launch = NULL,
    .post.resume = NULL
};

/* functions */

const struct vpc_hook *xilinx_mpsoc_default_vpc_hook(void)
{
    return &hook;
}

