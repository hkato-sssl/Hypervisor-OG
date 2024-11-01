// SPDX-License-Identifier: Apache-2.0
/*
 * vm/vm_launch.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t validate_parameters(struct vm *vm, const uint16_t vpc_no,
                                   const struct vpc_boot_configuration *boot)
{
    errno_t ret;

    if (vpc_no >= vm->nr_procs) {
        ret = -EINVAL;
    } else if (vm->vpcs[vpc_no] == NULL) {
        ret = -ENODEV;
    } else if (vpc_no == 0) {
        if (boot == NULL) {
            ret = -EINVAL;
        } else {
            ret = SUCCESS;
        }
    } else {
        if (boot != NULL) {
            ret = -EINVAL;
        } else {
            ret = SUCCESS;
        }
    }

    return ret;
}

/*
 * NOTE:
 * 本APIではAPIを呼び出した実プロセッサの番号が暗黙の引数として利用される。
 * APIを呼び出した実プロセッサと仮想プロセッサが関連付けされる。
 */
errno_t vm_launch(struct vm *vm, uint16_t vpc_no,
                  const struct vpc_boot_configuration *boot)
{
    errno_t ret;
    struct vpc *vpc;

    ret = validate_parameters(vm, vpc_no, boot);
    if (ret == SUCCESS) {
        vpc = vm_acquire_vpc(vm, vpc_no);
        if (vpc != NULL) {
            ret = vpc_launch(vpc, boot);
            if (ret == SUCCESS) {
                ret = vpc_event_loop(vpc);
            }
        } else {
            ret = -EPERM;
        }
    }

    return ret;
}
