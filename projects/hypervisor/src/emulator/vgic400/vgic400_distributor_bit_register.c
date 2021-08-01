/*
 * emulator/vgic400/vgic400_distributor_bit_register.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "driver/arm/gic400_io.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static uint32_t irq_no(uintptr_t reg, uintptr_t base)
{
    uint32_t no;

    no = (uint32_t)((reg - base) * 8);

    return no;
}

static uintptr_t reg_addr(uintptr_t base, uint32_t irq)
{
    uint32_t *reg;

    reg = (uint32_t *)base + (irq / 32);

    return (uintptr_t)reg;
}

static uint64_t read_bit(struct vgic400 *vgic, uint32_t base, uint32_t virq)
{
    uint64_t result;
    uint32_t irq;
    uint64_t d;
    uintptr_t reg;

    irq = vgic400_virq_to_irq(vgic, virq);
    if (irq < NR_GIC400_INTERRUPTS) {
        reg = reg_addr(base, irq);
        d = gic400_read_distributor(vgic->gic, reg);
        result = (d >> (irq % 32)) & 1;
    } else {
        result = 0;
    }

    return result;
}

static errno_t read_bit_register_w(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg, uintptr_t base)
{
    errno_t ret;
    uint32_t i;
    uint32_t virq;
    uint64_t d;

    virq = irq_no(reg, base);
    d = 0;

    for (i = 0; i < 32; ++i) {
        d |= read_bit(vgic, base, virq) << i;
        ++virq;
    }

    ret = insn_emulate_ldr(insn, d);

    return ret;
}

static void write_bit(struct vgic400 *vgic, uint32_t base, uint32_t virq)
{
    uint32_t bit;
    uint32_t irq;
    uintptr_t reg;

    irq = vgic400_virq_to_irq(vgic, virq);
    if (irq < NR_GIC400_INTERRUPTS) {
        reg = reg_addr(base, irq);
        bit = BIT(irq % 32);
        gic400_write_distributor(vgic->gic, reg, bit);
    }
}

static errno_t write_bit_register_w(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg, uintptr_t base)
{
    errno_t ret;
    uint32_t i;
    uint32_t virq;
    uint64_t d;

    d = insn_str_src_value(insn);
    virq = irq_no(reg, base);

    for (i = 0; i < 32; ++i) {
        if ((d & BIT(0)) != 0) {
            write_bit(vgic, base, virq);
        }
        ++virq;
        d >>= 1;
    }

    ret = insn_emulate_str(insn);

    return ret;
}

errno_t vgic400_distributor_bit_register(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg, uintptr_t base)
{
    errno_t ret;

    if (insn->type == INSN_TYPE_LDR) {
        if (is_aligned_word_access(insn)) {
            ret = read_bit_register_w(vgic, insn, reg, base);
        } else {
            ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
        }
    } else {
        if (is_aligned_word_access(insn)) {
            ret = write_bit_register_w(vgic, insn, reg, base);
        } else {
            ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
        }
    }

    return ret;
}

