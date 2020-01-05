/*
 * vm/vm_search_region_trap.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "hypervisor/vm.h"

/* defines */

#define IS_IN_RANGE(trap, addr)   (((trap)->ipa.addr <= (addr)) && ((addr) < ((trap)->ipa.addr + (trap)->ipa.size)))

/* types */

/* prototypes */

/* variables */

/* functions */

struct vm_region_trap *vm_search_region_trap(const struct vm *vm, uintptr_t addr)
{
    struct vm_region_trap *trap;

    trap = vm->emulator.trap.memory_region;
    while ((trap != NULL) && (! IS_IN_RANGE(trap, addr))) {
        trap = trap->next;
    }

    return trap;
}

