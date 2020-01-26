/*
 * vm/vm_vpc.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

struct vpc *vm_vpc(const struct vm *vm, uint32_t no)
{
    struct vpc *p;

    assert((vm != NULL) && (vm->vpcs != NULL));

    if (no < vm->nr_procs) {
        p = vm->vpcs[no];
    } else {
        p = NULL;
    }

    return p;
}
