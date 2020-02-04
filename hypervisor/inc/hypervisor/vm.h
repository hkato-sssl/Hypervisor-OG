/*
 * hypervisor/vm.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef HYPERVISOR_VM_H
#define HYPERVISOR_VM_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"
#include "driver/aarch64/stage2.h"
#include "hypervisor/vpc.h"

/* defines */

#define MAX_NR_VM_PROCESSORS    8
#define VM_NO_ASSIGN            0xffff

/* types */

struct vm_region_trap {
    struct vm_region_trap   *next;
    struct {
        bool                read;
        bool                write;
    } condition;
    uint64_t                ipa;
    uint64_t                pa;
    size_t                  size;
    struct {
        void                *arg;
        vpc_emulator_t      handler;
    } emulator;
};

struct vm {
    spin_lock_t                     lock;
    void                            *owner;
    uint16_t                        nr_procs;
    struct {
        uint16_t                    virtual[MAX_NR_VM_PROCESSORS];
        uint16_t                    physical[MAX_NR_VM_PROCESSORS];
    } proc_map;
    struct vpc                      *vpcs[MAX_NR_VM_PROCESSORS];
    struct aarch64_stage2           *stage2;

    struct {
        struct {
            struct vm_region_trap   *memory_region;
        } trap;
        struct {
            struct {
                const struct psci_ops   *ops;
            } aarch64;
            struct {
                const struct psci_ops   *ops;
            } aarch32;
            struct {
                uint8_t     report_unknown_function:1;
            } flag;
        } psci;
    } emulator;

};

struct vm_configuration {
    /* resources */
    void                            *owner;
    uint16_t                        nr_procs;
    struct aarch64_stage2           *stage2;
};

/* variables */

/* functions */

errno_t vm_initialize(struct vm *vm, const struct vm_configuration *config);
errno_t vm_register_vpc(struct vm *vm, struct vpc *vpc);
errno_t vm_launch(struct vm *vm, uint16_t vpc_no, const struct vpc_boot_configuration *boot);
errno_t vm_standby_processor(struct vm *vm, uint16_t vpc_no);
errno_t vm_init_local_context(struct vm *vm);
struct vpc *vm_aquire_vpc(struct vm *vm, uint16_t proc_no);
uint16_t vm_virtual_proc_no(struct vm *vm, uint16_t physical_no);
uint16_t vm_physical_proc_no(struct vm *vm, uint16_t virtual_no);

errno_t vm_register_region_trap(struct vm *vm, struct vm_region_trap *trap);
struct vm_region_trap *vm_search_region_trap(const struct vm *vm, uintptr_t addr);

/* inline functions */

static inline void vm_lock(struct vm *vm)
{
    spin_lock(&(vm->lock));
}

static inline void vm_unlock(struct vm *vm)
{
    spin_unlock(&(vm->lock));
}

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_VM_H */

