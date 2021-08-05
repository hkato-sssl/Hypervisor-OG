/*
 * emulator/vgic400/vgic400_distributor_byte_register.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/arm/gic400.h"
#include "driver/arm/gic400_io.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "hypervisor/vpc.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "vgic400_local.h"
#include <stdbool.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static uint32_t irq_no(uintptr_t reg, uintptr_t base)
{
    uint32_t no;

    no = (uint32_t)(reg - base);

    return no;
}

static uintptr_t reg_addr(uintptr_t base, uint32_t irq)
{
    return (base + irq);
}

static uint64_t read_byte(struct vgic400 *vgic, uint32_t base, uint32_t virq)
{
    uint64_t result;
    uint32_t irq;
    uintptr_t reg;

    irq = vgic400_virq_to_irq(vgic, virq);
    if (irq < NR_GIC400_INTERRUPTS) {
        reg = reg_addr(base, irq);
        result = gic400_read_distributor_b(vgic->gic, reg);
    } else {
        result = 0;
    }

    return result;
}

static errno_t read_byte_register_b(struct vgic400 *vgic,
                                    const struct insn *insn, uintptr_t reg,
                                    uintptr_t base)
{
    errno_t ret;
    uint32_t virq;
    uint64_t d;

    virq = irq_no(reg, base);
    d = read_byte(vgic, base, virq);

    ret = insn_emulate_ldr(insn, d);

    return ret;
}

static errno_t read_byte_register_w(struct vgic400 *vgic,
                                    const struct insn *insn, uintptr_t reg,
                                    uintptr_t base)
{
    errno_t ret;
    uint32_t i;
    uint32_t virq;
    uint64_t d;

    virq = irq_no(reg, base);
    d = 0;

    for (i = 0; i < 4; ++i) {
        d |= read_byte(vgic, base, virq) << (i * 8);
        ++virq;
    }

    ret = insn_emulate_ldr(insn, d);

    return ret;
}

static void write_byte(struct vgic400 *vgic, uint32_t base, uint32_t virq,
                       uint32_t d)
{
    uint32_t irq;
    uintptr_t reg;

    irq = vgic400_virq_to_irq(vgic, virq);
    if (irq < NR_GIC400_INTERRUPTS) {
        reg = reg_addr(base, irq);
        gic400_write_distributor_b(vgic->gic, reg, d);
    }
}
static errno_t write_byte_register_b(struct vgic400 *vgic,
                                     const struct insn *insn, uintptr_t reg,
                                     uintptr_t base)
{
    errno_t ret;
    uint32_t d;
    uint64_t virq;

    d = (uint32_t)insn_str_src_value(insn);
    virq = irq_no(reg, base);
    if (is_target_virq(vgic, virq)) {
        write_byte(vgic, base, virq, d);
    }

    ret = insn_emulate_str(insn);

    return ret;
}

static errno_t write_byte_register_w(struct vgic400 *vgic,
                                     const struct insn *insn, uintptr_t reg,
                                     uintptr_t base)
{
    errno_t ret;
    uint32_t i;
    uint32_t d;
    uint32_t virq;

    d = (uint32_t)insn_str_src_value(insn);
    virq = irq_no(reg, base);

    for (i = 0; i < 4; ++i) {
        if (is_target_virq(vgic, virq)) {
            write_byte(vgic, base, virq, d);
        }
        d >>= 8;
        ++virq;
    }

    ret = insn_emulate_str(insn);

    return ret;
}

errno_t vgic400_distributor_byte_register(struct vgic400 *vgic,
                                          const struct insn *insn,
                                          uintptr_t reg, uintptr_t base)
{
    errno_t ret;

    if (insn->type == INSN_TYPE_LDR) {
        if (insn->op.ldr.size == 1) {
            ret = read_byte_register_b(vgic, insn, reg, base);
        } else if (is_aligned_word_access(insn)) {
            ret = read_byte_register_w(vgic, insn, reg, base);
        } else {
            ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
        }
    } else {
        if (insn->op.str.size == 1) {
            ret = write_byte_register_b(vgic, insn, reg, base);
        } else if (is_aligned_word_access(insn)) {
            ret = write_byte_register_w(vgic, insn, reg, base);
        } else {
            ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
        }
    }

    return ret;
}
