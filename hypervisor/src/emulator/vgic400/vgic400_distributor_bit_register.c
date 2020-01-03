/*
 * emulator/vgic400/vgic400_distributor_bit_register.c
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

static uint64_t irq_no(uintptr_t reg, uintptr_t base)
{
    uint64_t no;

    no = (reg - base) * 8;

    return no;
}

static errno_t read_bit_register_w(struct vgic400 *vgic, const struct vpc_memory_access *access, uintptr_t reg, uintptr_t base)
{
    uint64_t d;
    uint64_t mask;
    uint64_t no;

    d = VGIC400_READ32(access->request.addr);
    no = irq_no(reg, base);
    mask = vgic->active.irq[no / 32];
    d &= mask;

    vpc_load_to_gpr_w(access, d);

    return SUCCESS;
}

static errno_t write_bit_register_w(struct vgic400 *vgic, const struct vpc_memory_access *access, uintptr_t reg, uintptr_t base)
{
    uint64_t d;
    uint64_t mask;
    uint64_t no;

    d = gpr_value(access);
    no = irq_no(reg, base);
    mask = vgic->active.irq[no / 32];
    d &= mask;
    VGIC400_WRITE32(access->request.addr, d);

    return SUCCESS;
}

errno_t vgic400_distributor_bit_register(struct vgic400 *vgic, const struct vpc_memory_access *access, uintptr_t reg, uintptr_t base)
{
    errno_t ret;

    if (access->request.type == VPC_READ_ACCESS) {
        if (is_aligned_word_access(access)) {
            ret = read_bit_register_w(vgic, access, reg, base);
        } else {
            ret = vgic400_distributor_error(access, ERR_MSG_UNAUTH);
        }
    } else {
        if (is_aligned_word_access(access)) {
            ret = write_bit_register_w(vgic, access, reg, base);
        } else {
            ret = vgic400_distributor_error(access, ERR_MSG_UNAUTH);
        }
    }

    return ret;
}

