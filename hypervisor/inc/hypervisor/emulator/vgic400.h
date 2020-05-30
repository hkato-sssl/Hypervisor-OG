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

#define NR_VGIC400_TARGET_MAPS          ((NR_GIC400_INTERRUPTS + 31) / 32)
#define NR_VGIC400_CPUS                 8
#define MAX_NR_VGIC400_LIST_REGISTERS   32

/* types */

struct vpc;
struct insn;

struct vgic400 {
    struct vm       *vm;
    struct gic400   *gic;
    struct {
        void        *virtif_control;
        void        *virtual_cpuif;
    } base;
    uint32_t        nr_list_registers;
    uint32_t        priority_mask;

    struct {
        uint32_t    virq[NR_VGIC400_TARGET_MAPS];
    } target;

    struct {
        uint8_t     priority[NR_GIC400_SGIS];
    } sgi[MAX_NR_VM_PROCESSORS];

    struct {
        uint32_t        template[NR_GIC400_PPIS];
    } ppi[MAX_NR_VM_PROCESSORS];

    struct {
        uint32_t        template[NR_GIC400_SPIS];
        struct {
            uint16_t    virtual[NR_GIC400_SPIS];
            uint16_t    physical[NR_GIC400_SPIS];
        } map;
    } spi;

    uint32_t    lr[MAX_NR_VM_PROCESSORS][MAX_NR_VGIC400_LIST_REGISTERS];
    uint32_t    iar[MAX_NR_VM_PROCESSORS][MAX_NR_VGIC400_LIST_REGISTERS];

    struct {
        struct vm_region_trap   distributor;
        struct vm_region_trap   cpuif;
    } trap;
};

struct vgic400_configuration {
    struct vm       *vm;
    struct gic400   *gic;
    struct {
        void        *virtif_control;
        void        *virtual_cpuif;
    } base;
    uint8_t         priority_mask;
    struct {
        bool        trap_cpuif;
    } boolean;
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

errno_t vgic400_initialize(struct vgic400 *vgic, const struct vgic400_configuration *config);
errno_t vgic400_configure_interrupt(struct vgic400 *vgic, const struct vgic400_interrupt_configuration *config);
errno_t vgic400_activate_virtual_cpuif(struct vgic400 *vgic);
errno_t vgic400_distributor_emulate_memory_access(const struct insn *insn, struct vgic400 *vgic);
errno_t vgic400_cpuif_emulate_memory_access(const struct insn *insn, struct vgic400 *vgic);
errno_t vgic400_inject_interrupt(struct vgic400 *vgic, struct vpc *vpc, uint32_t iar);
errno_t vgic400_inject_sgi(struct vgic400 *vgic, struct vpc *vpc, uint32_t iar);

errno_t vgic400_emulate_irq_exception(struct vpc *vpc, struct vgic400 *vgic);
errno_t vgic400_operate_maintenance_interrupt(struct vpc *vpc, struct vgic400 *vgic, uint32_t iar);

/* for debugging */

void gic400_dump_virtif_control(struct vgic400 *vgic);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_EMULATOR_VGIC400_H */

