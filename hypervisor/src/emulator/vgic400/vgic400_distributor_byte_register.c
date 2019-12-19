/*
 * emulator/vgic400/vgic400_distributor_access_byte_register.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "driver/arm/gic400.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t read_byte_register_b(struct vgic400 *vgic, const struct vpc_memory_access *access, uintptr_t reg, uintptr_t base)
{
    uintptr_t no;
    uint64_t d;

    no = reg - base;
    if (is_active_irq(vgic, no)) {
        gic400_lock(vgic->gic);
        d = VGIC400_READ8(access->request.addr);
        gic400_unlock(vgic->gic);
    } else {
        d = 0;
    }

    vpc_load_to_gpr_b(access, d);

    return SUCCESS;
}

static errno_t read_byte_register_w(struct vgic400 *vgic, const struct vpc_memory_access *access, uintptr_t reg, uintptr_t base)
{
    uint64_t d;
    uint64_t no;
    uint64_t mask;

    gic400_lock(vgic->gic);
    d = VGIC400_READ32(access->request.addr);
    gic400_unlock(vgic->gic);

    no = reg - base;
    mask = vgic400_quad_byte_mask(vgic, no);
    d &= mask;
    vpc_load_to_gpr_w(access, d);

    return SUCCESS;
}

static errno_t write_byte_register_b(struct vgic400 *vgic, const struct vpc_memory_access *access, uintptr_t reg, uintptr_t base)
{
    uint16_t no;
    uint64_t d;

    d = gpr_value(access);
    no = reg - base;
    if (is_active_irq(vgic, no)) {
        gic400_lock(vgic->gic);
        VGIC400_WRITE8(access->request.addr, d);
        gic400_unlock(vgic->gic);
    }

    return SUCCESS;
}

static errno_t write_byte_register_w(struct vgic400 *vgic, const struct vpc_memory_access *access, uintptr_t reg, uintptr_t base)
{
    uint32_t mask;
    uint64_t d;
    uint64_t d0;
    uint64_t no;

    d = gpr_value(access);
    no = reg - base;
    mask = vgic400_quad_byte_mask(vgic, no);
    d &= mask;

    gic400_lock(vgic->gic);
    d0 = VGIC400_READ32(access->request.addr);
    d |= d0 & ~mask;
    VGIC400_WRITE32(access->request.addr, d);
    gic400_unlock(vgic->gic);

    return SUCCESS;
}

errno_t vgic400_distributor_access_byte_register(struct vgic400 *vgic, const struct vpc_memory_access *access, uintptr_t reg, uintptr_t base)
{
    errno_t ret;

    if (access->request.type == VPC_READ_ACCESS) {
        if (access->request.size == 1) {
            ret = read_byte_register_b(vgic, access, reg, base);
        } else if (is_aligned_word_access(access)) {
            ret = read_byte_register_w(vgic, access, reg, base);
        } else {
            vgic400_distributor_error(access, ERR_MSG_UNAUTH);
            ret = -EPERM;
        }
    } else {
        if (access->request.size == 1) {
            ret = write_byte_register_b(vgic, access, reg, base);
        } else if (is_aligned_word_access(access)) {
            ret = write_byte_register_w(vgic, access, reg, base);
        } else {
            vgic400_distributor_error(access, ERR_MSG_UNAUTH);
            ret = -EPERM;
        }
    }

    return ret;
}

