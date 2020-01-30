/*
 * soc/xilinx/mpsoc/xilinx_mpsoc_launch_hook.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/arm/gic400.h"
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

errno_t xilinx_mpsoc_pervious_launch(struct vpc *vpc)
{
    errno_t ret;
    struct xilinx_mpsoc *chip;

    chip = vpc->vm->owner;

    /* 0xff -> the lowest priority */
    ret = gic400_set_priority_mask(chip->vgic400.gic, 0xff);
    if (ret == SUCCESS) {
        ret = vgic400_activate(&(chip->vgic400));
    }

    return ret;
}

