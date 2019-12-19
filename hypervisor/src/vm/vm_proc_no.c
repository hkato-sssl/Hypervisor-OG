/*
 * vm/vm_proc_no.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "hypervisor/vm.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

uint8_t vm_virtual_proc_no(struct vm *vm, uint8_t physical_no)
{
    return vm->proc_map.virtual[physical_no];
}

uint8_t vm_physical_proc_no(struct vm *vm, uint8_t virtual_no)
{
    return vm->proc_map.physical[virtual_no];
}

