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

/* types */

struct vm_region_trap {
    struct vm_region_trap          *next;
    uint64_t                       addr;
    size_t                         size;
    vpc_memory_access_emulator_t   emulator;
};

struct vm {
    spin_lock_t                     lock;
    uint32_t                        nr_procs;
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
    uint32_t                        nr_procs;
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
};

/* variables */

/* functions */

errno_t vm_configure(struct vm *vm, const struct vm_configuration *config);
errno_t vm_launch(struct vm *vm);
struct vpc *vm_vpc(const struct vm *vm, uint32_t index);

errno_t vm_register_region_trap(struct vm *vm, struct vm_region_trap *trap);
errno_t vm_emulate_aarch64_memory_access(struct vpc *vpc, const struct vpc_memory_access_request *req);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_VM_H */

