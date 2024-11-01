// SPDX-License-Identifier: Apache-2.0
/*
 * soc/xilinx/mpsoc/xilinx_mpsoc_assert_interrupt_vpc.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/arm/gic400.h"
#include "hypervisor/soc.h"
#include "hypervisor/soc/xilinx/mpsoc.h"
#include "hypervisor/vpc.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t xilinx_mpsoc_assert_interrupt_vpc(struct soc *soc,
                                          const struct vpc *vpc,
                                          uint16_t interrupt_no)
{
    errno_t ret;
    uint32_t targets;
    struct xilinx_mpsoc *mpsoc;

    mpsoc = soc->chip;

    if (interrupt_no < 16) {
        targets = 1UL << vm_physical_proc_no(vpc->vm, vpc->proc_no);
        ret = gic400_assert_sgi(mpsoc->vgic400.gic, targets, interrupt_no);
    } else if (interrupt_no < 32) {
        /* asserting PPI interrupts is not supported. */
        ret = -ENOTSUP;
    } else {
        ret = gic400_assert_spi(mpsoc->vgic400.gic, interrupt_no);
    }

    return ret;
}
