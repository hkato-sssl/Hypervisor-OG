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

#include "config/system.h"
#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/aarch64/stage2.h"
#include "hypervisor/vpc.h"

/* defines */

/* types */

struct vm {
    uint32_t                nr_vpcs;
    struct vpc              vpc[CONFIG_NR_CPUS];
    struct aarch64_stage2   *stage2;
};

struct vm_configuration {
    uint32_t                nr_vpcs;
    struct {
        uint64_t            *regs;
        struct {
            uint64_t        pc;
            uint64_t        sp;
        } gpr;
    } vpc[CONFIG_NR_CPUS];
    struct aarch64_stage2   *stage2;
};

/* variables */

/* functions */

errno_t vm_configure(struct vm *vm, struct vm_configuration const *config);
errno_t vm_launch(struct vm *vm);
errno_t vm_resume(struct vm *vm);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_VM_H */

