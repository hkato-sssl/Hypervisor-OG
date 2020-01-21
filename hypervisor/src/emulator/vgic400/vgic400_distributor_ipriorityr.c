/*
 * emulator/vgic400/vgic400_distributor_ipriorityr.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/arm/device/gic400.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static void update_priority_field(uint32_t *target, uint8_t priority)
{
    *target = (*target & ~(uint32_t)BITS(27, 20)) | ((uint32_t)priority << 20);
}

static errno_t update_priority(struct vgic400 *vgic, const struct insn *insn, uintptr_t no, uint8_t priority)
{
    uint32_t *p;

    if (is_target_irq(vgic, no)) {
        if (no < 16) {
            vgic->sgi[insn->vpc->proc_no].priority[no] = priority;
        } else if (no < 32) {
            p = &(vgic->ppi[insn->vpc->proc_no].template[no - 16]);
            update_priority_field(p, priority);
        } else {
            p = &(vgic->spi.template[no - 32]);
            update_priority_field(p, priority);
        }
    }

    return SUCCESS;
}

static errno_t update_priority_b(struct vgic400 *vgic, const struct insn *insn, uintptr_t no)
{
    errno_t ret;
    uint8_t priority;

    priority = (uint8_t)insn_str_src_value(insn);
    ret = update_priority(vgic, insn, no, priority);

    return ret;
}

static errno_t update_priority_w(struct vgic400 *vgic, const struct insn *insn, uintptr_t no)
{
    int i;
    uint32_t priority;

    priority = (uint32_t)insn_str_src_value(insn);
    for (i = 0; i < 4; ++i) {   
        update_priority(vgic, insn, no, (uint8_t)priority);
        ++no;
        priority >>= 8;
    }

    return SUCCESS;
}

errno_t vgic400_distributor_ipriorityr(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;
    uint32_t no;

    ret = vgic400_distributor_byte_register(vgic, insn, reg, GICD_IPRIORITYR(0));
    if ((ret == SUCCESS) && (insn->type == INSN_TYPE_STR)) {
        no = (reg - GICD_IPRIORITYR(0)) / 4;
        if (insn->op.str.size == 1) {
            ret = update_priority_b(vgic, insn, no);
        } else {
            ret = update_priority_w(vgic, insn, no);
        }
    }

    return ret;
}

