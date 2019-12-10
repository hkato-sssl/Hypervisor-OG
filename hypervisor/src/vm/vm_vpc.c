/*
 * vm/vm_vpc.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/assert.h"
#include "hypervisor/vm.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

struct vpc *vm_vpc(struct vm const *vm, uint32_t index)
{
    struct vpc *p;

    SYSTEM_ASSERT((vm != NULL) && (vm->vpcs != NULL) && (vm->nr_procs > index));

    p = vm->vpcs + index;

    return p;
}
