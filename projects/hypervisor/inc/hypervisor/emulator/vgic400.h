// SPDX-License-Identifier: Apache-2.0
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

#include "driver/arm/gic400.h"
#include "hypervisor/vm.h"
#include "lib/system/spin_lock.h"
#include <stdbool.h>
#include <stdint.h>

/* defines */

#define NR_VGIC400_TARGET_MAPS    ((NR_GIC400_INTERRUPTS + 31) / 32)
#define NR_VGIC400_CPUS           8
#define NR_VGIC400_PRIORITIES     16
#define NR_VGIC400_LIST_REGISTERS 4
#define NR_VGIC400_VIRTUAL_SPIS   32
#define NR_VGIC400_INTERRUPT_EVENTS                                       \
    ((NR_GIC400_SGIS * NR_VGIC400_CPUS) + NR_GIC400_PPIS + NR_GIC400_SPIS \
     + NR_VGIC400_VIRTUAL_SPIS)
#define VGIC400_NAME_LEN 16

/* types */

struct vpc;
struct insn;
struct vgic400;

struct vgic400_ops {
    errno_t (*el2_irq_handler)(struct vpc *vpc, struct vgic400 *vgic,
                               uint32_t iar);
};

struct vgic400_interrupt_event {
    uint8_t priority;
    uint8_t cpuid;
    uint16_t irq;
    uint32_t iar;
};

struct vgic400_interrupt_event_array {
    spin_lock_t lock;
    uint32_t num;
    uint64_t events[NR_VGIC400_INTERRUPT_EVENTS];
};

struct vgic400 {
    spin_lock_t lock;

    struct vm *vm;
    struct gic400 *gic;
    struct {
        void *virtif_control;
        void *virtual_cpuif;
    } base;
    uint32_t nr_list_registers;
    uint32_t template_typer;
    uint8_t priority_mask;

    const struct vgic400_ops *ops;

    struct {
        uint32_t irq[NR_VGIC400_TARGET_MAPS];
        uint32_t virq[NR_VGIC400_TARGET_MAPS];
    } target;

    struct {
        uint32_t list_register[NR_GIC400_SGIS];
        uint8_t priority[NR_GIC400_SGIS];
    } sgi[MAX_NR_VM_PROCESSORS];

    struct {
        uint32_t list_register[NR_GIC400_PPIS];
        uint8_t priority[NR_GIC400_PPIS];
    } ppi[MAX_NR_VM_PROCESSORS];

    struct {
        uint32_t list_register[NR_GIC400_SPIS];
        uint8_t priority[NR_GIC400_SPIS];
        struct {
            uint16_t virtual[NR_GIC400_SPIS];
            uint16_t physical[NR_GIC400_SPIS];
        } map;
    } spi;

    uint32_t lr[MAX_NR_VM_PROCESSORS][NR_VGIC400_LIST_REGISTERS];
    uint32_t iar[MAX_NR_VM_PROCESSORS][NR_VGIC400_LIST_REGISTERS];

    struct {
        struct vm_region_trap distributor;
        struct vm_region_trap cpuif[2];
    } trap;

    struct {
        bool half_priority;
    } boolean;

    struct vpc_event accept_event;

    struct {
        struct vgic400_interrupt_event_array *event_arrays[NR_VGIC400_CPUS];
    } interrupt;
};

struct vgic400_configuration {
    struct vm *vm;
    struct gic400 *gic;
    struct {
        void *virtif_control;
        void *virtual_cpuif;
    } base;

    const struct vgic400_ops *ops;

    struct {
        bool trap_cpuif;
        bool half_priority;
    } boolean;

    struct vgic400_interrupt_event_array *event_arrays[NR_VGIC400_CPUS];
};

struct vgic400_interrupt_configuration {
    struct {
        uint8_t hw     : 1;
        uint8_t group1 : 1;
    } flag;
    uint16_t virtual_id;
    uint16_t physical_id;
};

/* variables */

/* functions */

errno_t vgic400_initialize(struct vgic400 *vgic,
                           const struct vgic400_configuration *config);
errno_t vgic400_configure_interrupt(
    struct vgic400 *vgic, const struct vgic400_interrupt_configuration *config);
errno_t vgic400_activate_virtual_cpuif(struct vgic400 *vgic);
errno_t vgic400_distributor_emulate_memory_access(const struct insn *insn,
                                                  struct vgic400 *vgic);
errno_t vgic400_cpuif_emulate_memory_access(const struct insn *insn,
                                            struct vgic400 *vgic);
errno_t vgic400_inject_interrupt(struct vpc *, struct vgic400 *vgic,
                                 uint32_t iar);
errno_t vgic400_inject_interrupt_at(struct vpc *, struct vgic400 *vgic,
                                    uint32_t iar, uint32_t list_no);
errno_t vgic400_inject_sgi(struct vpc *, struct vgic400 *vgic, uint32_t iar);
errno_t vgic400_inject_sgi_at(struct vpc *, struct vgic400 *vgic, uint32_t iar,
                              uint32_t list_no);
errno_t vgic400_irq_handler(struct vpc *vpc, struct vgic400 *vgic);
errno_t vgic400_write_list_register(struct vpc *vpc, struct vgic400 *vgic400,
                                    uint32_t iar);
errno_t vgic400_create_interrupt_event(struct vpc *vpc, struct vgic400 *vgic,
                                       struct vgic400_interrupt_event *event,
                                       uint32_t iar);
errno_t
vgic400_inject_interrupt_event(struct vpc *vpc, struct vgic400 *vgic,
                               const struct vgic400_interrupt_event *event);
errno_t
vgic400_push_interrupt_event(struct vpc *vpc, struct vgic400 *vgic,
                             const struct vgic400_interrupt_event *event);
errno_t vgic400_pop_interrupt_event(struct vpc *vpc, struct vgic400 *vgic,
                                    struct vgic400_interrupt_event *event);
errno_t vgic400_update_list_registers(struct vpc *vpc, struct vgic400 *vgic);

/* for debugging */

void gic400_dump_virtif_control(struct vgic400 *vgic);
void gic400_dump_virtual_cpuif(struct vgic400 *vgic);
void gic400_dump_virtual_cpuif_at(struct vgic400 *vgic, uint8_t no);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_EMULATOR_VGIC400_H */
