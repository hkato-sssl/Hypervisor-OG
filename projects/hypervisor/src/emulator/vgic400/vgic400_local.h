/*
 * emulator/vgic400/vgic400_local.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef EMULATOR_VGIC400_VGIC400_LOCAL_H
#define EMULATOR_VGIC400_VGIC400_LOCAL_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "hypervisor/emulator/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "lib/bit.h"
#include "lib/system/assert.h"
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "lib/system/spin_lock.h"
#include <stdbool.h>
#include <stdint.h>

/* defines */

#define ERR_MSG_UNAUTH        "An unauthorized access has occured."
#define ERR_MSG_OOR           "A register is out of range."

#define VGIC400_READ8(a)      REG_READ8((a), 0)
#define VGIC400_READ32(a)     REG_READ32((a), 0)
#define VGIC400_WRITE8(a, d)  REG_WRITE8((a), 0, (d))
#define VGIC400_WRITE32(a, d) REG_WRITE32((a), 0, (d))

#define VGIC400_NO_ASSIGNED   0xffffUL

/* types */

/* variables */

/* inline functions */

static inline void gic400_write_virtif_control(struct vgic400 *vgic,
                                               uint32_t reg, uint32_t d)
{
    REG_WRITE32(vgic->base.virtif_control, reg, d);
}

static inline uint32_t gic400_read_virtif_control(struct vgic400 *vgic,
                                                  uint32_t reg)
{
    uint32_t d;

    d = REG_READ32(vgic->base.virtif_control, reg);

    return d;
}

static inline void gic400_write_virtual_cpuif(struct vgic400 *vgic,
                                              uint32_t reg, uint32_t d)
{
    REG_WRITE32(vgic->base.virtual_cpuif, reg, d);
}

static inline uint32_t gic400_read_virtual_cpuif(struct vgic400 *vgic,
                                                 uint32_t reg)
{
    uint32_t d;

    d = REG_READ32(vgic->base.virtual_cpuif, reg);

    return d;
}

static inline void vgic400_lock(struct vgic400 *vgic)
{
    spin_lock(&(vgic->lock));
}

static inline void vgic400_unlock(struct vgic400 *vgic)
{
    spin_unlock(&(vgic->lock));
}

static inline bool is_target_virq(const struct vgic400 *vgic, uint16_t virq)
{
    uint32_t bit;

    bit = BIT(virq % 32);

    return ((vgic->target.virq[virq / 32] & bit) != 0) ? true : false;
}

static inline bool is_target_irq(const struct vgic400 *vgic, uint16_t irq)
{
    uint32_t bit;

    bit = BIT(irq % 32);

    return ((vgic->target.irq[irq / 32] & bit) != 0) ? true : false;
}

static inline uint16_t vgic400_virq_to_irq(const struct vgic400 *vgic,
                                           uint32_t virq)
{
    uint16_t irq;

    if (virq < 32) {
        irq = is_target_virq(vgic, virq) ? virq : VGIC400_NO_ASSIGNED;
    } else if (vgic->boolean.virtual_spi && (virq >= vgic->virtual_spi.base_no)
               && (virq <= (vgic->virtual_spi.base_no + 31))) {
        irq = virq;
    } else if (virq < NR_GIC400_INTERRUPTS) {
        irq = vgic->spi.map.physical[virq - 32];
    } else {
        irq = VGIC400_NO_ASSIGNED;
    }

    return irq;
}

static inline uint16_t vgic400_irq_to_virq(const struct vgic400 *vgic,
                                           uint32_t irq)
{
    uint16_t virq;

    if (irq < 32) {
        virq = is_target_irq(vgic, irq) ? irq : VGIC400_NO_ASSIGNED;
    } else {
        virq = vgic->spi.map.virtual[irq - 32];
    }

    return virq;
}

static inline bool is_aligned_word_access(const struct insn *insn)
{
    bool ret;

    if (IS_ALIGNED(insn->op.ldr.pa, 4) && (insn->op.ldr.size == 4)) {
        ret = true;
    } else {
        ret = false;
    }

    return ret;
}

static inline bool is_virtual_spi_byte_register(const struct vgic400 *vgic,
                                                uintptr_t reg, uintptr_t base)
{
    bool ret;

    if (vgic->boolean.virtual_spi) {
        base += vgic->virtual_spi.base_no;
        ret = ((base <= reg) && (reg < (base + 32))) ? true : false;
    } else {
        ret = false;
    }

    return ret;
}

static inline bool is_virtual_spi_bit_register(const struct vgic400 *vgic,
                                               uintptr_t reg, uintptr_t base)
{
    bool ret;

    if (vgic->boolean.virtual_spi) {
        base += (vgic->virtual_spi.base_no / 32) * 4;
        ret = ((base <= reg) && (reg < (base + 4))) ? true : false;
    } else {
        ret = false;
    }

    return ret;
}

static inline uint64_t
vgic400_encode_interrupt_event(const struct vgic400_interrupt_event *event)
{
    uint64_t d;

    d = (uint64_t)(event->iar) << 32;
    d |= event->priority << 24;
    d |= event->irq << 8;
    d |= event->cpuid;

    return d;
}

static inline void
vgic400_decode_interrupt_event(struct vgic400_interrupt_event *event,
                               uint64_t d)
{
    event->cpuid = (uint8_t)d;
    event->irq = (uint16_t)(d >> 8);
    event->priority = (uint8_t)(d >> 24);
    event->iar = d >> 32;
}

static inline bool is_less(uint64_t a, uint64_t b)
{
    return ((uint32_t)a < (uint32_t)b);
}

/* functions */

errno_t vgic400_distributor_ro_word_register(struct vgic400 *vgic,
                                             const struct insn *insn);
errno_t vgic400_distributor_bit_register(struct vgic400 *vgic,
                                         const struct insn *insn, uintptr_t reg,
                                         uintptr_t base);
errno_t vgic400_distributor_byte_register(struct vgic400 *vgic,
                                          const struct insn *insn,
                                          uintptr_t reg, uintptr_t base);
errno_t vgic400_distributor_ctlr(struct vgic400 *vgic, const struct insn *insn);
errno_t vgic400_distributor_typer(struct vgic400 *vgic,
                                  const struct insn *insn);
errno_t vgic400_distributor_igroupr(struct vgic400 *vgic,
                                    const struct insn *insn);
errno_t vgic400_distributor_isenabler(struct vgic400 *vgic,
                                      const struct insn *insn, uintptr_t reg);
errno_t vgic400_distributor_icenabler(struct vgic400 *vgic,
                                      const struct insn *insn, uintptr_t reg);
errno_t vgic400_distributor_ispendr(struct vgic400 *vgic,
                                    const struct insn *insn, uintptr_t reg);
errno_t vgic400_distributor_icpendr(struct vgic400 *vgic,
                                    const struct insn *insn, uintptr_t reg);
errno_t vgic400_distributor_isactiver(struct vgic400 *vgic,
                                      const struct insn *insn, uintptr_t reg);
errno_t vgic400_distributor_icactiver(struct vgic400 *vgic,
                                      const struct insn *insn, uintptr_t reg);
errno_t vgic400_distributor_spisr(struct vgic400 *vgic, const struct insn *insn,
                                  uintptr_t reg);
errno_t vgic400_distributor_ipriorityr(struct vgic400 *vgic,
                                       const struct insn *insn, uintptr_t reg);
errno_t vgic400_distributor_itargetsr(struct vgic400 *vgic,
                                      const struct insn *insn, uintptr_t reg);
errno_t vgic400_distributor_sgir(struct vgic400 *vgic, const struct insn *insn);
errno_t vgic400_distributor_icfgr(struct vgic400 *vgic, const struct insn *insn,
                                  uintptr_t reg);

errno_t vgic400_cpuif_write_word_register(const struct insn *insn,
                                          struct vgic400 *vgic);
errno_t vgic400_cpuif_read_word_register(const struct insn *insn,
                                         struct vgic400 *vgic);

uint64_t vgic400_p2v_cpu_map_b(uint64_t src, const struct vm *vm);
uint64_t vgic400_p2v_cpu_map_w(uint64_t src, const struct vm *vm);
uint64_t vgic400_v2p_cpu_map_b(uint64_t src, const struct vm *vm);
uint64_t vgic400_v2p_cpu_map_w(uint64_t src, const struct vm *vm);

errno_t vgic400_distributor_error(const struct insn *insn, const char *msg);

int vgic400_list_register(struct vgic400 *vgic);
errno_t vgic400_accept_virtual_spi(struct vpc *vpc, struct vgic400 *vgic);
errno_t vgic400_expose_virtual_spi(struct vpc *vpc, struct vgic400 *vgic);

errno_t vgic400_create_interrupt_event(struct vpc *vpc, struct vgic400 *vgic,
                                       struct vgic400_interrupt_event *event,
                                       uint32_t iar);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* EMULATOR_VGIC400_VGIC400_LOCAL_H */
