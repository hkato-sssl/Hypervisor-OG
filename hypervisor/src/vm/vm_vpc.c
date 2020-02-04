/*
 * vm/vm_vpc.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

struct vpc *vm_vpc(const struct vm *vm, uint16_t vpc_no)
{
    struct vpc *p;

    if (vpc_no < vm->nr_procs) {
        p = vm->vpcs[vpc_no];
    } else {
        p = NULL;
    }

    return p;
}

