// SPDX-License-Identifier: Apache-2.0
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

#include "lib/system/errno.h"
#include <stdbool.h>
#include <stdint.h>

/* defines */

/* types */

struct soc;
struct vpc;
struct insn;

/* variables */

/* functions */

errno_t xilinx_mpsoc_previous_launch_hook(struct vpc *vpc);
errno_t xilinx_mpsoc_emulate_irq_exception(struct vpc *vpc);
errno_t xilinx_mpsoc_emulate_hvc(const struct insn *insn);
errno_t xilinx_mpsoc_emulate_smc(const struct insn *insn);
errno_t xilinx_mpsoc_emulate_psci(struct vpc *vpc);
errno_t xilinx_mpsoc_emulate_tz(struct vpc *vpc);
errno_t xilinx_mpsoc_emulate_tz_asm(uint64_t *regs);
errno_t xilinx_mpsoc_assert_interrupt_vpc(struct soc *soc,
                                          const struct vpc *vpc,
                                          uint16_t interrupt_no);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* SOC_XILINX_MPSOC_MPSOC_LOCAL_H */
