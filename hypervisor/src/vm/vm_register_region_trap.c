/*
 * vm/vm_register_region_trap.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system.h"
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t append_region_trap(struct vm_region_trap *head, struct vm_region_trap *region)
{
    errno_t ret;
    struct vm_region_trap *p;

    ret = -EINVAL;
    for (p = head; p != region; p = p->next) {
        if (p->next == NULL) {
            p->next = region;
            region->next = NULL;
            ret = SUCCESS;
            break;
        }
    }

    return ret;
}

errno_t register_region_trap(struct vm *vm, struct vm_region_trap *region)
{
    errno_t ret;
    struct vm_region_trap *head;

    head = vm->emulator.trap.memory_region;
    if (head != NULL) {
        ret = append_region_trap(head, region);
    } else {
        vm->emulator.trap.memory_region = region; 
        region->next = NULL;
        ret = SUCCESS;
    }

    return ret;
}

errno_t vm_register_region_trap(struct vm *vm, struct vm_region_trap *region)
{
    errno_t ret;

    ret = system_validate_stack_region(region, sizeof(*region));
    if (ret != SUCCESS) {
        ret = register_region_trap(vm, region);
    } else {
        ret = -EINVAL;
    }

    return ret;
}
