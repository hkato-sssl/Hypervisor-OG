/*
 * emulator/vgic400/vgic400_distributor_icfgr.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
#include "driver/arm/device/gic400.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static uint32_t irq_no(uintptr_t reg)
{
    uintptr_t no;

    no = (uint32_t)((reg - GICD_ICFGR(0)) * 4);

    return no;
}

static uintptr_t reg_addr(uint32_t irq)
{
    return (uintptr_t)GICD_ICFGR(irq / 16);
}

static uint64_t read_icfgr(const struct vgic400 *vgic, uint32_t virq)
{
    uint64_t result;
    uint32_t d;
    uint32_t irq;
    uintptr_t reg;

    irq = vgic400_virq_to_irq(vgic, virq);
    if (irq < NR_GIC400_INTERRUPTS) {
        reg = reg_addr(irq);
        d = gic400_read_distributor(vgic->gic, reg);
        result = (d >> ((irq % 16) * 2)) & 0x03;
    } else {
        result = 0;
    }

    return result;
}

static errno_t read_icfgr_w(const struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;
    uint32_t i;
    uint32_t virq;
    uint64_t d;

    virq = irq_no(reg);
    d = 0;

    for (i = 0; i < 16; ++i) {
        d |= read_icfgr(vgic, virq) << (i * 2);
        ++virq;
    }

    ret = insn_emulate_ldr(insn, d);

    return ret;
}

static void write_icfgr(const struct vgic400 *vgic, uint32_t virq, uint32_t d)
{
    uint32_t d0;
    uint32_t irq;
    uint32_t mask;
    uint32_t shift_ct;
    uintptr_t reg;

    irq = vgic400_virq_to_irq(vgic, virq);
    if (irq < NR_GIC400_INTERRUPTS) {
        shift_ct = (irq % 16) * 2;
        mask = ~((uint32_t)BITS(1, 0) << shift_ct);
        d = (d & BITS(1, 0)) << shift_ct;
        reg = reg_addr(irq);

        d0 = gic400_read_distributor(vgic->gic, reg);
        d |= d0 & mask;
        gic400_write_distributor(vgic->gic, reg, d);
    }
}

static errno_t write_icfgr_w(const struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;
    uint32_t i;
    uint32_t virq;
    uint64_t d;

    d = insn_str_src_value(insn);
    virq = irq_no(reg);

    /*
     * 本関数は GICD_ICFGRn に対して read/modify/write 操作を実施する為、
     * 同じレジスタを操作する gic400_configure_interrupt() との間で排他
     * 制御を行う。
     */

    gic400_lock(vgic->gic);

    for (i = 0; i < 16; ++i) {
        write_icfgr(vgic, virq, (uint32_t)d);
        ++virq;
        d >>= 2;
    }

    gic400_unlock(vgic->gic);

    ret = insn_emulate_str(insn);

    return ret;
}

static errno_t read_virtual_icfgr(const struct insn *insn)
{
    errno_t ret;

    ret = insn_emulate_ldr(insn, 0);

    return ret;
}

static errno_t write_virtual_icfgr(const struct insn *insn)
{
    errno_t ret;

    ret = insn_emulate_str(insn);   /* Ignore a write operation. */

    return ret;
}

static bool is_virtual_icfgr(const struct vgic400 *vgic, uintptr_t reg)
{
    bool ret;
    uintptr_t base;

    if (vgic->boolean.virtual_spi) {
        base = GICD_ICFGR(0) + (vgic->virtual_spi.base_no / 16) * 4;
        ret = (reg == base) ? true : false;
    } else {
        ret = false;
    }

    return ret;
}

errno_t vgic400_distributor_icfgr(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;

    if (insn->type == INSN_TYPE_LDR) {
        if (is_aligned_word_access(insn)) {
            if (is_virtual_icfgr(vgic, reg)) {
                ret = read_virtual_icfgr(insn);
            } else {
                ret = read_icfgr_w(vgic, insn, reg);
            }
        } else {
            ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
        }
    } else {
        if (is_aligned_word_access(insn)) {
            if (is_virtual_icfgr(vgic, reg)) {
                ret = write_virtual_icfgr(insn);
            } else {
                ret = write_icfgr_w(vgic, insn, reg);
            }
        } else {
            ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
        }
    }

    return ret;
}

