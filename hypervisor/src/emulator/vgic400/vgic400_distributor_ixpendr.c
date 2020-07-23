/*
 * emulator/vgic400/vgic400_distributor_ixpendr.c
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

errno_t read_virtual_ixpendr(struct vgic400 *vgic, const struct insn *insn)
{
    errno_t ret;

    /* Read as zero. */

    ret = insn_emulate_ldr(insn, 0);

    return ret;
}

errno_t write_virtual_ispendr(struct vgic400 *vgic, const struct insn *insn)
{
    errno_t ret;
    uint32_t d;

    d = (uint32_t)insn_str_src_value(insn);
    if (d != 0) {
        vgic400_lock(vgic);
        vgic->virtual_spi.pendr |= d;
        vgic400_unlock(vgic);

        ret = vgic400_encourage_virtual_spi_interrupt(vgic);
        if (ret == SUCCESS) {
            ret = insn_emulate_str(insn);
        }
    } else {
        ret = insn_emulate_str(insn);
    }

    return ret;
}

errno_t write_virtual_icpendr(struct vgic400 *vgic, const struct insn *insn)
{
    errno_t ret;
    uint32_t d;
    uint32_t mask;

    d = (uint32_t)insn_str_src_value(insn);
    if (d != 0) {
        mask = ~d;
        vgic400_lock(vgic);
        vgic->virtual_spi.pendr &= mask;
        vgic400_unlock(vgic);
    }

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

static bool is_virtual_ispendr(struct vgic400 *vgic, uintptr_t reg)
{
    return is_virtual_spi(vgic, reg, GICD_ISPENDR(0));
}

static bool is_virtual_icpendr(struct vgic400 *vgic, uintptr_t reg)
{
    return is_virtual_spi(vgic, reg, GICD_ICPENDR(0));
}

errno_t vgic400_distributor_ispendr(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;

    if (is_virtual_ispendr(vgic, reg)) {
        if (insn->type == INSN_TYPE_LDR) {
            ret = read_virtual_ixpendr(vgic, insn);
        } else {
            ret = write_virtual_ispendr(vgic, insn);
        }
    } else {
        ret = vgic400_distributor_bit_register(vgic, insn, reg, GICD_ISPENDR(0));
    }

    return ret;
}

errno_t vgic400_distributor_icpendr(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;

    if (is_virtual_icpendr(vgic, reg)) {
        if (insn->type == INSN_TYPE_LDR) {
            ret = read_virtual_ixpendr(vgic, insn);
        } else {
            ret = write_virtual_icpendr(vgic, insn);
        }
    } else {
        ret = vgic400_distributor_bit_register(vgic, insn, reg, GICD_ICPENDR(0));
    }

    return ret;
}

