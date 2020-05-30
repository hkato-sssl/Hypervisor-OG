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

static errno_t update_priority(struct vgic400 *vgic, const struct insn *insn, uintptr_t virq, uint8_t priority)
{
    uint32_t *p;

    priority &= vgic->priority_mask;

    if (is_target_virq(vgic, virq)) {
        if (virq < 16) {
            vgic->sgi[insn->vpc->proc_no].priority[virq] = priority;
        } else if (virq < 32) {
            p = &(vgic->ppi[insn->vpc->proc_no].template[virq - 16]);
            update_priority_field(p, priority);
        } else {
            p = &(vgic->spi.template[virq - 32]);
            update_priority_field(p, priority);
        }
    }

    return SUCCESS;
}

static errno_t update_priority_b(struct vgic400 *vgic, const struct insn *insn, uintptr_t virq)
{
    errno_t ret;
    uint8_t priority;

    priority = (uint8_t)insn_str_src_value(insn);
    ret = update_priority(vgic, insn, virq, priority);

    return ret;
}

static errno_t update_priority_w(struct vgic400 *vgic, const struct insn *insn, uintptr_t virq)
{
    int i;
    uint32_t priority;

    priority = (uint32_t)insn_str_src_value(insn);
    for (i = 0; i < 4; ++i) {   
        update_priority(vgic, insn, virq, (uint8_t)priority);
        ++virq;
        priority >>= 8;
    }

    return SUCCESS;
}

errno_t vgic400_distributor_ipriorityr(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;
    uint32_t virq;

    ret = vgic400_distributor_byte_register(vgic, insn, reg, GICD_IPRIORITYR(0));
    if ((ret == SUCCESS) && (insn->type == INSN_TYPE_STR)) {
        virq = reg - GICD_IPRIORITYR(0);
        if (insn->op.str.size == 1) {
            ret = update_priority_b(vgic, insn, virq);
        } else {
            ret = update_priority_w(vgic, insn, virq);
        }
    }

    return ret;
}

