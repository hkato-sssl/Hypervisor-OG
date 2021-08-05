/*
 * vm/vm_search_region_trap.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "hypervisor/vm.h"
#include <stdint.h>

/* defines */

#define IS_IN_RANGE(trap, addr)    \
    (((trap)->memory.pa <= (addr)) \
     && ((addr) < ((trap)->memory.pa + (trap)->memory.size)))

/* types */

/* prototypes */

/* variables */

/* functions */

static bool match(void *element, void *arg)
{
    bool result;
    struct vm_region_trap *trap;

    trap = element;
    result = IS_IN_RANGE(trap, (uintptr_t)arg) ? true : false;

    return result;
}

struct vm_region_trap *vm_search_region_trap(const struct vm *vm,
                                             uintptr_t addr)
{
    struct vm_region_trap *trap;

    trap = slist_search_element(&(vm->emulator.trap.memory_region), match,
                                (void *)addr);

    return trap;
}
