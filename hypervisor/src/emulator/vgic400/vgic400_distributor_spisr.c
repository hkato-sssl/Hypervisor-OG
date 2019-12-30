/*
 * emulator/vgic400/vgic400_distributor_spisr.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/arm/device/gic400.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t read_spisr_w(struct vgic400 *vgic, const struct vpc_memory_access *access, uintptr_t reg)
{
    uint32_t d;
    uint32_t idx;
    uint32_t mask;

    idx = (reg - GICD_SPISR(0)) / 4 + 1;
    mask = vgic->active.irq[idx];
    d = VGIC400_READ32(access->request.addr);
    d &= mask;

    vpc_load_to_gpr_w(access, d);

    return SUCCESS;
}

errno_t vgic400_distributor_spisr(struct vgic400 *vgic, const struct vpc_memory_access *access, uintptr_t reg)
{
    errno_t ret;

    /* a write operation will be ignored */

    if (access->request.type == VPC_READ_ACCESS) {
        if (is_aligned_word_access(access)) {
            ret = read_spisr_w(vgic, access, reg);
        } else {
            ret = vgic400_distributor_error(access, ERR_MSG_UNAUTH);
        }
    } else {
        if (is_aligned_word_access(access)) {
            ret = SUCCESS;
        } else {
            ret = vgic400_distributor_error(access, ERR_MSG_UNAUTH);
        }
    }

    return ret;
}

