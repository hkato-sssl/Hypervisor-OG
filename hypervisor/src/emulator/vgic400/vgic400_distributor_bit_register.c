/*
 * emulator/vgic400/vgic400_distributor_access_bit_register.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t read_bit_register_b(struct vgic400 *vgic, const struct vpc_memory_access *access, uintptr_t reg, uintptr_t base)
{
    uint64_t d;
    uint64_t mask;
    uint64_t offset;

    d = VGIC400_READ8(access->request.addr);
    offset = reg - base;
    mask = vgic->active.irq[offset / 32];
    mask >>= ((offset & BITS(1, 0)) * 8);
    d &= mask;

    vpc_load_to_gpr_b(access, d);

    return SUCCESS;
}

static errno_t read_bit_register_w(struct vgic400 *vgic, const struct vpc_memory_access *access, uintptr_t reg, uintptr_t base)
{
    uint64_t d;
    uint64_t mask;
    uint64_t offset;

    d = VGIC400_READ32(access->request.addr);
    offset = reg - base;
    mask = vgic->active.irq[offset / 32];
    d &= mask;

    vpc_load_to_gpr_w(access, d);

    return SUCCESS;
}

static errno_t write_bit_register_b(struct vgic400 *vgic, const struct vpc_memory_access *access, uintptr_t reg, uintptr_t base)
{
    uint64_t d;
    uint64_t mask;
    uint64_t offset;

    d = gpr_value(access);
    offset = reg - base;
    mask = vgic->active.irq[offset / 32];
    mask >>= ((offset & BITS(1, 0)) * 8);
    d &= mask;
    VGIC400_WRITE8(access->request.addr, d);

    return SUCCESS;
}

static errno_t write_bit_register_w(struct vgic400 *vgic, const struct vpc_memory_access *access, uintptr_t reg, uintptr_t base)
{
    uint64_t d;
    uint64_t mask;
    uint64_t offset;

    d = gpr_value(access);
    offset = reg - base;
    mask = vgic->active.irq[offset / 32];
    d &= mask;
    VGIC400_WRITE32(access->request.addr, d);

    return SUCCESS;
}

errno_t vgic400_distributor_access_bit_register(struct vgic400 *vgic, const struct vpc_memory_access *access, uintptr_t reg, uintptr_t base)
{
    errno_t ret;

    if (access->request.type == VPC_READ_ACCESS) {
        if (access->request.size == 1) {
            ret = read_bit_register_b(vgic, access, reg, base);
        } else if (access->request.size == 4) {
            ret = read_bit_register_w(vgic, access, reg, base);
        } else {
            vgic400_distributor_error(access, ERR_MSG_UNAUTH);
            ret = -EPERM;
        }
    } else {
        if (access->request.size == 1) {
            ret = write_bit_register_b(vgic, access, reg, base);
        } else if (access->request.size == 4) {
            ret = write_bit_register_w(vgic, access, reg, base);
        } else {
            vgic400_distributor_error(access, ERR_MSG_UNAUTH);
            ret = -EPERM;
        }
    }

    return ret;
}

