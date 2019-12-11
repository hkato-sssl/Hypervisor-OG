/*
 * hypervisor/vgic400.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef HYPERVISOR_VGIC400_H
#define HYPERVISOR_VGIC400_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "driver/arm/gic400.h"

/* defines */

/* types */

struct vm;

struct vgic400 {
    struct vm       *owner;
    struct {
        uint8_t     irq[NR_GIC400_INTERRUPTS / 8];
    } active;

    struct {
    } cpuif;

    struct {
    } dist;
};

struct vgic400_configuration {
    struct {
        uint16_t num;
        uint16_t *array;
    } irq;
};

/* variables */

/* inline functions */

static inline bool vgic400_test_active_irq(struct vgic400 *vgic, uint16_t irq)
{
    uint8_t bit;

    bit = 1 << (irq & 7);
    
    return ((vgic->active.irq[irq / 8] & bit) != 0) ? true : false;
}

/* functions */

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_VGIC400_H */

