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
#include "hypervisor/vm.h"

/* defines */

#define NR_VGIC400_STATUS_MAPS          ((NR_GIC400_INTERRUPTS + 31) / 32)
#define NR_VGIC400_CPUS                 8
#define MAX_NR_VGIC400_LIST_REGISTERS   32

/* types */

struct vpc;
struct insn;

struct vgic400 {
    struct vm       *owner;
    struct gic400   *gic;
    void            *reg_base;
    uint32_t        nr_list_registers;

    struct {
        uint32_t    irq[NR_VGIC400_STATUS_MAPS];
    } target;

    uint8_t         sgi_priority[MAX_NR_VM_PROCESSORS][NR_GIC400_SGIS];

    struct {
        uint32_t    ppi[MAX_NR_VM_PROCESSORS][NR_GIC400_PPIS];
        uint32_t    spi[NR_GIC400_SPIS];
    } template;

    uint32_t        list[MAX_NR_VGIC400_LIST_REGISTERS];
};

struct vgic400_configuration {
    struct vm       *owner;
    struct gic400   *gic;
    void            *reg_base;
};

struct vgic400_interrupt_configuration {
    struct {
        uint8_t     hw:1;
        uint8_t     group1:1;
    } flag;
    uint16_t        virtual_id;
    uint16_t        physical_id;
};

/* variables */

/* functions */

errno_t vgic400_configure(struct vgic400 *vgic, const struct vgic400_configuration *config);
errno_t vgic400_configure_interrupt(struct vgic400 *vgic, const struct vgic400_interrupt_configuration *config);
errno_t vgic400_distributor_emulate_memory_access(const struct insn *insn, struct vgic400 *vgic);
errno_t vgic400_emulate_irq_exception(struct vgic400 *vgic, struct vpc *vpc);
errno_t vgic400_inject_interrupt(struct vgic400 *vgic, uint32_t no);
errno_t vgic400_inject_sgi(struct vgic400 *vgic, uint32_t no);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_EMULATOR_VGIC400_H */

