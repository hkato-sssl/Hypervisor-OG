/*
 * vm/vm_launch.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "hypervisor/vm.h"
#include "vm_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t launch(struct vm *vm)
{
    errno_t ret;

    vm->boolean.launched = true;
    ret = vm_event_loop(vm, 0, &(vm->boot));
    vm->boolean.launched = false;

    return ret;
}

errno_t vm_launch(struct vm *vm)
{
    errno_t ret;

    if ((vm != NULL) && (! vm->boolean.launched)) {
        ret = launch(vm);
    } else {
        ret = -EINVAL;
    }

    return ret;
}
