/*
 * vm/vm_emulate_memory_access.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static bool is_in_region(const struct vm_region_trap *trap, const struct vpc_memory_access *access)
{
    bool ret;

    if ((trap->addr <= access->request.addr) &&
        ((access->request.addr + access->request.size) <= (trap->addr + trap->size))) {
        ret = true;
    } else {
        ret = false;
    }

    return ret;
}

static struct vm_region_trap *search_trap(struct vm *vm, const struct vpc_memory_access *access)
{
    struct vm_region_trap *trap;

    trap = vm->emulator.trap.memory_region;
    while ((trap != NULL) && (! is_in_region(trap, access))) {
        trap = trap->next;
    }

    return trap;
}

errno_t vm_emulate_memory_access(struct vm *vm, const struct vpc_memory_access *access)
{
    errno_t ret;
    struct vm_region_trap *trap;

    trap = search_trap(vm, access);
    if (trap != NULL) {
        ret = (trap->emulator)(access);
    } else {
        ret = -ENOSYS;
    }

    return ret;
}
