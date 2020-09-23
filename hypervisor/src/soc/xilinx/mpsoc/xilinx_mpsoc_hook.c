/*
 * soc/xilinx/mpsoc/xilinx_mpsoc_hook.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/vgic400.h"
#include "hypervisor/soc/xilinx/mpsoc.h"
#include "mpsoc_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t xilinx_mpsoc_previous_launch_hook(struct vpc *vpc)
{
    errno_t ret;
    struct xilinx_mpsoc *chip;

    chip = vpc->vm->soc->chip;

    ret = vgic400_activate_virtual_cpuif(&(chip->vgic400));
    if (ret == SUCCESS) {
        ret = gic400_set_priority_mask(chip->vgic400.gic, 0xff);
    }

    return ret;
}

