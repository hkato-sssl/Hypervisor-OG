/*
 * vm/vm_launch_secondary.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t launch_secondary(struct vm *vm, struct vpc *vpc)
{
    errno_t ret;

    ret = vpc_wait_request(vpc);
    if (ret == SUCCESS) {
        ret = vpc_launch(vpc, NULL);
        if (ret == SUCCESS) {
            ret = vpc_event_loop(vpc);
        }
    }

    return ret;
}

errno_t vm_launch_secondary(struct vm *vm, uint16_t vpc_no)
{
    errno_t ret;

    struct vpc *vpc;

    if (vpc_no < vm->nr_procs) {
        vpc = vm_acquire_vpc(vm, vpc_no);
        if (vpc != NULL) {
            ret = launch_secondary(vm, vpc);
        } else {
            ret = -EPERM;
        }
    } else {
        ret = -EINVAL;
    }

    return ret;
}

