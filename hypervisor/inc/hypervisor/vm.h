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

struct vm {
    spin_lock_t             lock;
    uint32_t                nr_procs;
    struct vpc              *vpcs;
    struct aarch64_stage2   *stage2;
};

struct vm_configuration {
    /* resources */
    uint32_t                nr_procs;
    struct {
        struct vpc          *addr;
        size_t              size;
    } vpcs;
    struct {
        uint64_t            *addr;
        size_t              size;
    } regs;
    struct aarch64_stage2   *stage2;

    /* boot status */
    struct {
        enum vpc_arch       arch;
        uint64_t            pc;
        uint64_t            sp;
    } boot;
};

/* variables */

/* inline functions */

static inline struct vpc *vm_vpc(struct vm const *vm, uint32_t index)
{
    return (index < vm->nr_procs) ? &(vm->vpcs[index]) : NULL;
}

/* functions */

errno_t vm_configure(struct vm *vm, struct vm_configuration const *config);
errno_t vm_launch(struct vm *vm);
errno_t vm_resume(struct vm *vm);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_VM_H */
