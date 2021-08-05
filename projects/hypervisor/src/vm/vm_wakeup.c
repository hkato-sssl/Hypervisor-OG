/*
 * vm/vm_wakeup.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "hypervisor/soc.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "lib/bit.h"
#include "vm_local.h"
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t validate_parameters(struct vm *vm, const uint16_t proc_no,
                                   const struct vpc_boot_configuration *boot)
{
    errno_t ret;

    if (proc_no < vm->nr_procs) {
        if (vm->vpcs[proc_no] != NULL) {
            ret = SUCCESS;
        } else {
            ret = -ENODEV;
        }
    } else {
        ret = -EINVAL;
    }

    return ret;
}

errno_t vm_wakeup(struct vm *vm, uint16_t proc_no,
                  const struct vpc_boot_configuration *boot)
{
    errno_t ret;

    vm_lock(vm);

    ret = validate_parameters(vm, proc_no, boot);
    if (ret == SUCCESS) {
        ret = vpc_wakeup_processor(vm->vpcs[proc_no], boot);
    }

    vm_unlock(vm);

    return ret;
}
