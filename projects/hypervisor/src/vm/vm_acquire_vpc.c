// SPDX-License-Identifier: Apache-2.0
/*
 * vm/vm_acquire_vpc.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/system/cpu.h"
#include "hypervisor/vm.h"
#include "lib/system/errno.h"
#include "vm_local.h"
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

struct vpc *acquire_vpc(struct vm *vm, uint16_t proc_no)
{
    struct vpc *vpc;
    uint16_t physical_no;

    physical_no = cpu_no();

    vm_lock(vm);
    if ((vm->proc_map.virtual[physical_no] == VM_NO_ASSIGN)
        && (vm->proc_map.physical[proc_no] == VM_NO_ASSIGN)
        && (vm->vpcs[proc_no] != NULL)) {
        vpc = vm->vpcs[proc_no];
        vm->proc_map.virtual[physical_no] = proc_no;
        vm->proc_map.physical[proc_no] = physical_no;
    } else {
        vpc = NULL;
    }
    vm_unlock(vm);

    return vpc;
}

struct vpc *vm_acquire_vpc(struct vm *vm, uint16_t proc_no)
{
    struct vpc *vpc;

    if (proc_no < vm->nr_procs) {
        vpc = acquire_vpc(vm, proc_no);
    } else {
        vpc = NULL;
    }

    return vpc;
}
