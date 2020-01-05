/*
 * emulator/vgic400/vgic400_distributor_spisr.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/arm/device/gic400.h"
#include "hypervisor/vpc.h"
#include "hypervisor/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t read_spisr_w(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    uint32_t d;
    uint32_t idx;
    uint32_t mask;

    idx = (reg - GICD_SPISR(0)) / 4 + 1;
    mask = vgic->active.irq[idx];
    d = VGIC400_READ32(insn->op.ldr.ipa);
    d &= mask;

    vpc_load_to_gpr_w(insn, d);

    return SUCCESS;
}

errno_t vgic400_distributor_spisr(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;

    /* a write operation will be ignored */

    if (insn->type == INSN_TYPE_LDR) {
        if (is_aligned_word_access(insn)) {
            ret = read_spisr_w(vgic, insn, reg);
        } else {
            ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
        }
    } else {
        if (is_aligned_word_access(insn)) {
            ret = SUCCESS;
        } else {
            ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
        }
    }

    return ret;
}

