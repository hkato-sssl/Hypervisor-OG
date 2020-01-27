/*
 * vm/vm_map_proc_no.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include "lib/system/errno.h"
#include "driver/aarch64.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t map_proc_no(struct vm *vm, const struct vpc *vpc)
{
    uint8_t phys_no;

    phys_no = (uint8_t)aarch64_cpu_no();

    vm_lock(vm);
    vm->proc_map.virtual[phys_no] = vpc->proc_no;
    vm->proc_map.physical[vpc->proc_no] = phys_no;
    vm_unlock(vm);

    return SUCCESS;
}

errno_t vm_map_proc_no(struct vm *vm, const struct vpc *vpc)
{
    errno_t ret;

    assert((vm != NULL) && (vpc != NULL));

    if ((vm->nr_procs < vpc->proc_no) && (vm->vpcs[vpc->proc_no] == vpc)) {
        ret = map_proc_no(vm, vpc);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

