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

static bool is_in_region(const struct vm_region_trap *trap, const struct vpc_memory_access_request *req)
{
    bool ret;

    if ((trap->addr <= req->addr) &&
        ((req->addr + req->size) <= (trap->addr + trap->size))) {
        ret = true;
    } else {
        ret = false;
    }

    return ret;
}

static struct vm_region_trap *search_trap(struct vpc *vpc, const struct vpc_memory_access_request *req)
{
    struct vm_region_trap *trap;

    trap = vpc->owner->emulator.trap.memory_region;
    while ((trap != NULL) && (! is_in_region(trap, req))) {
        trap = trap->next;
    }

    return trap;
}

errno_t vm_emulate_memory_access(struct vpc *vpc, const struct vpc_memory_access_request *req)
{
    errno_t ret;
    struct vm_region_trap *trap;

    trap = search_trap(vpc, req);
    if (trap != NULL) {
        ret = (trap->emulator)(vpc, req);
    } else {
        ret = -ENOSYS;
    }

    return ret;
}
