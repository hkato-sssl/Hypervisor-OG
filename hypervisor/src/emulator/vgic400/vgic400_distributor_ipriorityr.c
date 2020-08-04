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

static uint32_t virtual_ipriorityr_index(struct vgic400 *vgic, uintptr_t reg)
{
    uint32_t idx;

    idx = reg - GICD_IPRIORITYR(0) - vgic->virtual_spi.base_no;

    return idx;
}

static uint8_t *virtual_ipriorityr(struct vgic400 *vgic, uintptr_t reg)
{
    uint8_t *ipriorityr;
    uint32_t idx;

    idx = virtual_ipriorityr_index(vgic, reg);
    ipriorityr = vgic->virtual_spi.ipriorityr + idx;

    return ipriorityr;
}

static bool is_writable(struct vgic400 *vgic, uintptr_t virq, uint8_t priority)
{
    bool ret;

    if (is_target_virq(vgic, virq)) {
        if ((priority == 0) && vgic->boolean.ignore_priority0) {
            ret = false;
        } else {
            ret = true;
        }
    } else {
        ret = false;
    }

    return ret;
}

static void update_priority_field(uint32_t *target, uint8_t priority)
{
    *target = (*target & ~(uint32_t)BITS(27, 20)) | ((uint32_t)priority << 20);
}

static errno_t update_priority(struct vgic400 *vgic, const struct insn *insn, uintptr_t virq, uint8_t priority)
{
    uint32_t *p;

    priority &= vgic->priority_mask;

    if (is_writable(vgic, virq, priority)) {
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

static errno_t read_virtual_ipriorityr_b(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;
    uintptr_t d;
    uint8_t *ipriorityr;

    ipriorityr = virtual_ipriorityr(vgic, reg);

    vgic400_lock(vgic);
    d = *ipriorityr;
    vgic400_unlock(vgic);

    ret = insn_emulate_ldr(insn, d);

    return ret;
}

static errno_t read_virtual_ipriorityr_w(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;
    uint8_t *ipriorityr;
    uintptr_t d;

    ipriorityr = virtual_ipriorityr(vgic, reg);

    vgic400_lock(vgic);
    d = ((uintptr_t)(ipriorityr[3]) << 24) | ((uintptr_t)(ipriorityr[2]) << 16) | ((uintptr_t)(ipriorityr[1]) << 8) | ipriorityr[0];
    vgic400_unlock(vgic);

    ret = insn_emulate_ldr(insn, d);

    return ret;
}

static errno_t write_virtual_ipriorityr_b(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;
    uint8_t d;
    uint32_t idx;
    uint32_t pending;

    d = (uint8_t)insn_str_src_value(insn);
    d &= vgic->priority_mask;
    idx = virtual_ipriorityr_index(vgic, reg);

    vgic400_lock(vgic);

    pending = vgic->virtual_spi.ipendr & vgic->virtual_spi.ienabler;
    vgic->virtual_spi.ipriorityr[idx] = d;
    if ((d < vgic->priority_mask) && ((pending & BIT(idx)) != 0)) {
        ret = vgic400_accept_virtual_spi_interrupt(insn->vpc, vgic);
    } else {
        ret = SUCCESS;
    }

    vgic400_unlock(vgic);

    if (ret == SUCCESS) {
        ret = insn_emulate_str(insn);
    }

    return ret;
}

static errno_t write_virtual_ipriorityr_w(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;
    bool accept;
    uint32_t i;
    uint32_t idx;
    uint8_t priority;
    uint8_t *ipriorityr;
    uint32_t d;
    uint32_t pending;

    accept = false;
    d = (uint32_t)insn_str_src_value(insn);
    idx = virtual_ipriorityr_index(vgic, reg);
    ipriorityr = vgic->virtual_spi.ipriorityr + idx;

    vgic400_lock(vgic);

    pending = vgic->virtual_spi.ipendr & vgic->virtual_spi.ienabler;
    for (i = 0; i < 4; ++i) {
        priority = (uint8_t)d & vgic->priority_mask;
        ipriorityr[i] = priority;
        d >>= 8;
        if ((priority < vgic->priority_mask) && ((pending & BIT(idx + i)) != 0)) {
            accept = true;
        }
    }

    if (accept) {
        ret = vgic400_accept_virtual_spi_interrupt(insn->vpc, vgic);
    } else {
        ret = SUCCESS;
    }

    vgic400_unlock(vgic);

    if (ret == SUCCESS) {
        ret = insn_emulate_str(insn);
    }

    return ret;
}

static errno_t read_virtual_ipriorityr(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;

    if (insn->op.ldr.size == 1) {
        ret = read_virtual_ipriorityr_b(vgic, insn, reg);
    } else if (insn->op.ldr.size == 4) {
        ret = read_virtual_ipriorityr_w(vgic, insn, reg);
    } else {
        ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
    }

    return ret;
}

static errno_t write_virtual_ipriorityr(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;

    if (insn->op.str.size == 1) {
        ret = write_virtual_ipriorityr_b(vgic, insn, reg);
    } else if (insn->op.str.size == 4) {
        ret = write_virtual_ipriorityr_w(vgic, insn, reg);
    } else {
        ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
    }

    return ret;
}

static bool is_virtual_ipriorityr(struct vgic400 *vgic, uintptr_t reg)
{
    bool ret;

    ret = is_virtual_spi_byte_register(vgic, reg, GICD_IPRIORITYR(0));

    return ret;
}

errno_t vgic400_distributor_ipriorityr(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;
    uint32_t virq;

    if (is_virtual_ipriorityr(vgic, reg)) {
        if (insn->type == INSN_TYPE_LDR) {
            ret = read_virtual_ipriorityr(vgic, insn, reg);
        } else {
            ret = write_virtual_ipriorityr(vgic, insn, reg);
        }
    } else {
        ret = vgic400_distributor_byte_register(vgic, insn, reg, GICD_IPRIORITYR(0));
        if ((ret == SUCCESS) && (insn->type == INSN_TYPE_STR)) {
            virq = reg - GICD_IPRIORITYR(0);
            if (insn->op.str.size == 1) {
                ret = update_priority_b(vgic, insn, virq);
            } else {
                ret = update_priority_w(vgic, insn, virq);
            }
        }
    }

    return ret;
}

