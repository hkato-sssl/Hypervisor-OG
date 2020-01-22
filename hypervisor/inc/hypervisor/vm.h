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
    uint8_t                         nr_procs;
    struct {
        uint8_t                     virtual[MAX_NR_VM_PROCESSORS];
        uint8_t                     physical[MAX_NR_VM_PROCESSORS];
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
    void                            *owner;
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

