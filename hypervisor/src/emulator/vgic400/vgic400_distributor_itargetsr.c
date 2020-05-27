/*
 * emulator/vgic400/vgic400_distributor_itargetsr.c
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
    return (uint32_t)(reg - GICD_ITARGETSR(0));
}

static uintptr_t reg_addr(uint32_t irq)
{
    return (uintptr_t)GICD_ITARGETSR(0) + irq;
}

static uint64_t read_itargetsr(const struct vgic400 *vgic, const struct vm *vm, uint32_t virq)
{
    uint64_t result;
    uint32_t irq;
    uint64_t d;
    uintptr_t reg;

    irq = vgic400_virq_to_irq(vgic, virq);
    if (irq < NR_GIC400_INTERRUPTS) {
        reg = reg_addr(irq);
        d = gic400_read_distributor_b(vgic->gic, reg);
        result = vgic400_p2v_cpu_map_b(d, vm);
    } else {
        result = 0;
    }

    return result;
}

static errno_t read_itargetsr_b(const struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;
    uint32_t virq;
    uint64_t d;

    virq = irq_no(reg);
    d = read_itargetsr(vgic, insn->vpc->vm, virq);

    ret = insn_emulate_ldr(insn, d);

    return ret;
}

static errno_t read_itargetsr_w(const struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;
    uint32_t i;
    uint32_t virq;
    uint64_t d;

    virq = irq_no(reg);
    d = 0;

    for (i = 0; i < 4; ++i) {
        d |= read_itargetsr(vgic, insn->vpc->vm, virq) << (i * 8);
        ++virq;
    }

    ret = insn_emulate_ldr(insn, d);

    return ret;
}

static void write_itargetsr(const struct vgic400 *vgic, const struct vm *vm, uint32_t virq, uint64_t d)
{
    uint32_t irq;
    uintptr_t reg;

    irq = vgic400_virq_to_irq(vgic, virq);
    if (irq < NR_GIC400_INTERRUPTS) {
        d = vgic400_v2p_cpu_map_b(d, vm);
        reg = reg_addr(irq);
        gic400_write_distributor_b(vgic->gic, reg, (uint32_t)d);
    }
}

static errno_t write_itargetsr_b(const struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;
    uint32_t virq;
    uint64_t d;

    d = insn_str_src_value(insn);
    virq = irq_no(reg);
    write_itargetsr(vgic, insn->vpc->vm, virq, d);

    ret = insn_emulate_str(insn);

    return ret;
}

static errno_t write_itargetsr_w(const struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;
    uint32_t i;
    uint32_t virq;
    uint64_t d;

    d = insn_str_src_value(insn);
    virq = irq_no(reg);

    for (i = 0; i < 4; ++i) {
        write_itargetsr(vgic, insn->vpc->vm, virq, d);
        ++virq;
        d >>= 8;
    }

    ret = insn_emulate_str(insn);

    return ret;
}

errno_t vgic400_distributor_itargetsr(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg)
{
    errno_t ret;

    if (insn->type == INSN_TYPE_LDR) {
        if (insn->op.ldr.size == 1) {
            ret = read_itargetsr_b(vgic, insn, reg);
        } else if (is_aligned_word_access(insn)) {
            ret = read_itargetsr_w(vgic, insn, reg);
        } else {
            ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
        }
    } else {
        if (insn->op.str.size == 1) {
            ret = write_itargetsr_b(vgic, insn, reg);
        } else if (is_aligned_word_access(insn)) {
            ret = write_itargetsr_w(vgic, insn, reg);
        } else {
            ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
        }
    }

    return ret;
}

