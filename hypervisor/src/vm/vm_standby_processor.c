/*
 * vm/vm_standby_processor.c
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

static errno_t standby_processor(struct vm *vm, struct vpc *vpc)
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

errno_t vm_standby_processor(struct vm *vm, uint8_t proc_no)
{
    errno_t ret;

    struct vpc *vpc;

    if (proc_no >= vm->nr_procs) {
        vpc = vm_aquire_vpc(vm, proc_no);
        if (vpc != NULL) {
            ret = standby_processor(vm, vpc);
        } else {
            ret = -EPERM;
        }
    } else {
        ret = -EINVAL;
    }

    return ret;
}

