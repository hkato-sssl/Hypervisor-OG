/*
 * vm/vm_launch.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include "hypervisor/vm.h"
#include "vm_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t launch(struct vm *vm, const struct vpc_boot_configuration *boot)
{
    errno_t ret;
    struct vpc *vpc;

    /* boot the primary processor */

    vpc = vm_vpc(vm, 0);    /* boot the primary processor */
    ret = vpc_launch(vpc, boot);

    return ret;
}

errno_t vm_launch(struct vm *vm, const struct vpc_boot_configuration *boot)
{
    errno_t ret;

    assert((vm != NULL) && (boot != NULL));

    vm_lock(vm);
    if (! vm->boolean.launched) {
        vm->boolean.launched = true;
        vm_unlock(vm);
        ret = launch(vm, boot);
    } else {
        vm_unlock(vm);
        ret = -EINVAL;
    }

    return ret;
}

