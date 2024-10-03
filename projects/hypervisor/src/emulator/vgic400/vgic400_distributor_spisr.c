/*
 * emulator/vgic400/vgic400_distributor_spisr.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/arm//gic400_io.h"
#include "driver/arm/device/gic400.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "hypervisor/vpc.h"
#include "lib/system/errno.h"
#include "vgic400_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static uint32_t irq_no(uintptr_t reg)
{
    uint32_t no;

    no = (uint32_t)((reg - GICD_SPISR(0)) * 8 + 32);

    return no;
}

static uintptr_t reg_addr(uint32_t irq)
{
    return GICD_SPISR(irq / 32 - 1);
}

static uint64_t read_spisr(struct vgic400 *vgic, uint32_t virq)
{
    uint64_t result;
    uint64_t d;
    uint32_t irq;
    uintptr_t reg;

    irq = vgic400_virq_to_irq(vgic, virq);
    if (irq < NR_GIC400_INTERRUPTS) {
        reg = reg_addr(irq);
        d = gic400_read_distributor(vgic->gic, reg);
        result = (d >> (irq % 32)) & 1;
    } else {
        result = 0;
    }

    return result;
}

static errno_t read_spisr_w(struct vgic400 *vgic, const struct insn *insn,
                            uintptr_t reg)
{
    errno_t ret;
    uint32_t i;
    uint32_t virq;
    uint64_t d;

    virq = irq_no(reg);
    d = 0;

    for (i = 0; i < 32; ++i) {
        d |= read_spisr(vgic, virq) << i;
        ++virq;
    }

    ret = insn_emulate_ldr(insn, d);

    return ret;
}

errno_t vgic400_distributor_spisr(struct vgic400 *vgic, const struct insn *insn,
                                  uintptr_t reg)
{
    errno_t ret;

    /* a write operation will be ignored */

    if (insn->type == INSN_TYPE_LDR) {
        if (is_aligned_word_access(insn)) {
            ret = read_spisr_w(vgic, insn, reg);
        } else {
            ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
        }
    } else {
        if (is_aligned_word_access(insn)) {
            ret = insn_emulate_str(insn);
        } else {
            ret = vgic400_distributor_error(insn, ERR_MSG_UNAUTH);
        }
    }

    return ret;
}
