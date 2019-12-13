/*
 * vm/vm_vpc.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/stdc/assert.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

struct vpc *vm_vpc(const struct vm *vm, uint32_t index)
{
    struct vpc *p;

    assert((vm != NULL) && (vm->vpcs != NULL) && (vm->nr_procs > index));

    p = vm->vpcs + index;

    return p;
}
