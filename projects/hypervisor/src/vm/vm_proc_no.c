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

uint16_t vm_virtual_proc_no(struct vm *vm, uint16_t physical_no)
{
    uint16_t no;

    if (physical_no < MAX_NR_VM_PROCESSORS) {
        no = vm->proc_map.virtual[physical_no];
    } else {
        no = VM_NO_ASSIGN;
    }

    return no;
}

uint16_t vm_physical_proc_no(struct vm *vm, uint16_t virtual_no)
{
    uint16_t no;

    if (virtual_no < MAX_NR_VM_PROCESSORS) {
        no = vm->proc_map.physical[virtual_no];
    } else {
        no = VM_NO_ASSIGN;
    }

    return no;
}

