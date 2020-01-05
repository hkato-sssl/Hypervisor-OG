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
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"
#include "driver/aarch64/stage2.h"
#include "hypervisor/vpc.h"

/* defines */

#define VM_MAX_NR_PROCS             8
#define UNIT_VM_TRAP_REGION         4096

/* types */

struct vm_region_trap {
    struct vm_region_trap   *next;
    struct {
        uint64_t            addr;
        size_t              size;
    } ipa;
    struct {
        void                *arg;
        vpc_emulator_t      handler;
    } emulator;
};

struct vm {
    spin_lock_t                     lock;
    uint8_t                         nr_procs;
    struct {
        uint8_t                     virtual[VM_MAX_NR_PROCS];
        uint8_t                     physical[VM_MAX_NR_PROCS];
    } proc_map;
    struct vpc                      *vpcs;
    struct aarch64_stage2           *stage2;
    struct {
        bool                        launched;
    } boolean;
    struct vpc_boot_configuration   boot;

    struct {
        struct {
            struct vm_region_trap   *memory_region;
        } trap;
    } emulator;
};

struct vm_configuration {
    /* resources */
    uint8_t                         nr_procs;
    struct {
        struct vpc                  *addr;
        size_t                      size;
    } vpcs;
    struct {
        uint64_t                    *addr;
        size_t                      size;
    } regs;
    struct aarch64_stage2           *stage2;
    struct vpc_boot_configuration   boot;
    struct {
        struct {
            const struct vpc_exception_ops  *ops;
        } exception;
    } vpc;
};

/* variables */

/* functions */

errno_t vm_configure(struct vm *vm, const struct vm_configuration *config);
errno_t vm_launch(struct vm *vm);
struct vpc *vm_vpc(const struct vm *vm, uint32_t index);
uint8_t vm_virtual_proc_no(struct vm *vm, uint8_t physical_no);
uint8_t vm_physical_proc_no(struct vm *vm, uint8_t virtual_no);

errno_t vm_register_region_trap(struct vm *vm, struct vm_region_trap *trap);
struct vm_region_trap *vm_search_region_trap(const struct vm *vm, uintptr_t addr);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_VM_H */

