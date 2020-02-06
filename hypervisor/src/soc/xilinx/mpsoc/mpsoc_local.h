/*
 * soc/xilinx/mpsoc/mpsoc_local.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef SOC_XILINX_MPSOC_MPSOC_LOCAL_H
#define SOC_XILINX_MPSOC_MPSOC_LOCAL_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>
#include <stdbool.h>
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"

/* defines */

/* types */

/* variables */

/* functions */

errno_t xilinx_mpsoc_previous_launch_hook(struct vpc *vpc);
errno_t xilinx_mpsoc_emulate_irq_exception(struct vpc *vpc);
bool xilinx_mpsoc_test_executable_region(void *chip, uintptr_t addr);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* SOC_XILINX_MPSOC_MPSOC_LOCAL_H */

