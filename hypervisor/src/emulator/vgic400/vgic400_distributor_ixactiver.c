/*
 * emulator/vgic400/vgic400_distributor_ixactiver.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include "lib/system/errno.h"
#include "driver/arm/device/gic400.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t read_virtual_ixactiver(struct vgic400 *vgic, const struct insn *insn)
{
    errno_t ret;

    /* Read as zero. */

    ret = insn_emulate_ldr(insn, 0);

    return ret;
}

errno_t write_virtual_isactiver(struct vgic400 *vgic, const struct insn *insn)
{
    errno_t ret;

    /* Ignore a write operation. */

    ret = insn_emulate_str(insn);

    return ret;
}

errno_t write_virtual_icactiver(struct vgic400 *vgic, const struct insn *insn)
{
    errno_t ret;

    /* Ignore a write operation. */

    ret = insn_emulate_str(insn);

    return ret;
}

static bool is_virtual_spi(struct vgic400 *vgic, uintptr_t reg, uintptr_t base)
{
    bool ret;
    uintptr_t vreg;

    if (vgic->boolean.virtual_spi) {
        vreg = base + (vgic->virtual_spi.base_no / 32) * 4;
        ret = (vreg == reg) ? true : false;
    } else {
        ret = false;
    }

    return ret;
}

static bool is_virtual_isactiver(struct vgic400 *vgic, uintptr_t reg)
{
    return is_virtual_spi(vgic, reg, GICD_ISACTIVER(0));
}

static bool is_virtual_icactiver(struct vgic400 *vgic, uintptr_t reg)
{
    return is_virtual_spi(vgic, reg, GICD_ICACTIVER(0));
}

errno_t vgic400_distributor_isactiver(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;

    if (is_virtual_isactiver(vgic, reg)) {
        if (insn->type == INSN_TYPE_LDR) {
            ret = read_virtual_ixactiver(vgic, insn);
        } else {
            ret = write_virtual_isactiver(vgic, insn);
        }
    } else {
        ret = vgic400_distributor_bit_register(vgic, insn, reg, GICD_ISACTIVER(0));
    }

    return ret;
}

errno_t vgic400_distributor_icactiver(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;

    if (is_virtual_icactiver(vgic, reg)) {
        if (insn->type == INSN_TYPE_LDR) {
            ret = read_virtual_ixactiver(vgic, insn);
        } else {
            ret = write_virtual_icactiver(vgic, insn);
        }
    } else {
        ret = vgic400_distributor_bit_register(vgic, insn, reg, GICD_ICACTIVER(0));
    }

    return ret;
}

