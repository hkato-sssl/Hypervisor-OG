/*
 * hypervisor/emulator/vgic400.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef HYPERVISOR_EMULATOR_VGIC400_H
#define HYPERVISOR_EMULATOR_VGIC400_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>
#include <stdbool.h>
#include "driver/arm/gic400.h"

/* defines */

#define NR_GIC400_STATUS_MAPS   ((NR_GIC400_INTERRUPTS + 31) / 32)

/* types */

struct vm;
struct vpc;
struct insn;
struct gic400;

struct vgic400 {
    struct vm       *owner;
    struct gic400   *gic;

    struct {
        uint32_t    irq[NR_GIC400_STATUS_MAPS];
    } target;
};

struct vgic400_configuration {
    struct vm       *owner;
    struct gic400   *gic;
    struct {
        uint16_t    num;
        uint16_t    *array;
    } irq;
};

/* variables */

/* inline functions */

static inline bool vgic400_is_target_irq(struct vgic400 *vgic, uint16_t irq)
{
    uint32_t bit;

    bit = 1 << (irq & 31);
    
    return ((vgic->target.irq[irq / 32] & bit) != 0) ? true : false;
}

/* functions */

errno_t vgic400_configure(struct vgic400 *vgic, const struct vgic400_configuration *config);
errno_t vgic400_distributor_emulate_memory_access(const struct insn *insn, struct vgic400 *vgic);
errno_t vgic400_emulate_irq(struct vgic400 *vgic, struct vpc *vpc);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_EMULATOR_VGIC400_H */

