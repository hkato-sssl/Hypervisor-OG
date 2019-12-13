/*
 * vm/vm_event_loop.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include "lib/system/errno.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t event_loop(struct vpc *vpc)
{
    errno_t ret;

    ret = vpc_emulate_exception(vpc);
    while (ret == SUCCESS) {
        ret = vpc_resume(vpc);
        if (ret == SUCCESS) {
            ret = vpc_emulate_exception(vpc);
        }
    }

    return ret;
}

errno_t vm_event_loop(struct vm *vm, uint32_t proc_no, const struct vpc_boot_configuration *boot)
{
    errno_t ret;
    struct vpc *vpc;

    assert((vm != NULL) && (proc_no < vm->nr_procs) && (boot != NULL));

    vpc = vm_vpc(vm, proc_no);
    ret = vpc_launch(vpc, boot);
    if (ret == SUCCESS) {
        ret = event_loop(vpc);
    }

    return ret;
}

