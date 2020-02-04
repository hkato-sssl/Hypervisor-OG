/*
 * soc/xilinx/mpsoc/xilinx_mpsoc_emulate_irq_exception.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/vgic400.h"
#include "hypervisor/soc/xilinx/mpsoc.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t xilinx_mpsoc_emulate_irq_exception(struct vpc *vpc)
{
    errno_t ret;
    struct xilinx_mpsoc *chip;

    chip = vpc->vm->owner;
    ret = vgic400_emulate_irq_exception(vpc, &(chip->vgic400));

    return ret;
}
