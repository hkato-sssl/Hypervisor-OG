/*
 * vm/vm_register_vpc.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include <assert.h>
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"
#include "vm_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t register_vpc(struct vm *vm, struct vpc *vpc)
{
    vm->vpcs[vpc->proc_no] = vpc;

    return SUCCESS;
}

errno_t vm_register_vpc(struct vm *vm, struct vpc *vpc)
{
    errno_t ret;

    assert((vm != 0) && (vpc != NULL));

    vm_lock(vm);
    if (vpc->proc_no >= vm->nr_procs) {
        ret = -EINVAL;
    } else if (vm->vpcs[vpc->proc_no] != NULL) {
        ret = -EEXIST;
    } else if (vm != vpc->vm) {
        ret = -EPERM;
    } else {
        ret = register_vpc(vm, vpc);
    }
    vm_unlock(vm);

    return ret;
}

