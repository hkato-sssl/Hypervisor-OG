/*
 * emulator/vgic400/vgic400_distributor_ipriorityr.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/arm/device/gic400.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "lib/system/errno.h"
#include "vgic400_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static bool is_writable(struct vgic400 *vgic, uintptr_t virq)
{
    bool ret;

    ret = is_target_virq(vgic, virq);

    return ret;
}

static void update_priority_field(uint32_t *target, uint8_t priority)
{
    *target = (*target & ~(uint32_t)BITS(27, 20)) | ((uint32_t)priority << 20);
}

static errno_t update_priority(struct vgic400 *vgic, const struct insn *insn,
                               uintptr_t virq, uint8_t priority)
{
    uint32_t *p;

    if (is_writable(vgic, virq)) {
        priority &= vgic->priority_mask;

        if (virq < 16) {
            vgic->sgi[insn->vpc->proc_no].priority[virq] = priority;
            p = &(vgic->sgi[insn->vpc->proc_no].list_register[virq]);
        } else if (virq < 32) {
            vgic->ppi[insn->vpc->proc_no].priority[virq - 16] = priority;
            p = &(vgic->ppi[insn->vpc->proc_no].list_register[virq - 16]);
        } else {
            vgic->spi.priority[virq - 32] = priority;
            p = &(vgic->spi.list_register[virq - 32]);
        }
        update_priority_field(p, priority);
    }

    return SUCCESS;
}

static errno_t update_priority_b(struct vgic400 *vgic, const struct insn *insn,
                                 uintptr_t virq)
{
    errno_t ret;
    uint8_t priority;

    priority = (uint8_t)insn_str_src_value(insn);
    ret = update_priority(vgic, insn, virq, priority);

    return ret;
}

static errno_t update_priority_w(struct vgic400 *vgic, const struct insn *insn,
                                 uintptr_t virq)
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

errno_t vgic400_distributor_ipriorityr(struct vgic400 *vgic,
                                       const struct insn *insn, uintptr_t reg)
{
    errno_t ret;
    uint32_t virq;

    ret = vgic400_distributor_byte_register(vgic, insn, reg,
                                            GICD_IPRIORITYR(0));
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
