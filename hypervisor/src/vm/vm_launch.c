/*
 * vm/vm_launch.c
 *
 * (C) 2019 Hidekazu Kato
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

static errno_t validate_parameters(struct vm *vm, const uint8_t vpc_no, const struct vpc_boot_configuration *boot)
{
    errno_t ret;

    if (vpc_no < vm->nr_procs) {
        if (vm->vpcs[vpc_no] != NULL) {
            if (vpc_no == 0) {
                if (boot != NULL) {
                    ret = SUCCESS;
                } else {
                    ret = -EINVAL;
                }
            } else {
                ret = SUCCESS;
            }
        } else {
            ret = -ENODEV;
        }
    } else {
        ret = -EINVAL;
    }

    return ret;
}

/*
 * NOTE:
 * 本APIではAPIを呼び出した実プロセッサの番号が暗黙の引数として利用される。
 * APIを呼び出した実プロセッサと仮想プロセッサが関連付けされる。
 */
errno_t vm_launch(struct vm *vm, uint8_t vpc_no, const struct vpc_boot_configuration *boot)
{
    errno_t ret;
    struct vpc *vpc;

    ret = validate_parameters(vm, vpc_no, boot);
    if (ret == SUCCESS) {
        vpc = vm_aquire_vpc(vm, vpc_no);
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

