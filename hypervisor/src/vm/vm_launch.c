/*
 * vm/vm_launch.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include "driver/system/cpu.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"
#include "vm_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static struct vpc *activate_vpc(struct vm *vm, const uint8_t vpc_no)
{
    struct vpc *vpc;
    uint8_t physical_no;

    physical_no = cpu_no();
    if ((vm->proc_map.physical[vpc_no] == VM_NO_ASSIGN) && (vm->proc_map.virtual[physical_no] == VM_NO_ASSIGN)) {
        vm->proc_map.physical[vpc_no] = physical_no;
        vm->proc_map.virtual[physical_no] = vpc_no;
        vpc = vm->vpcs[vpc_no];
    } else {
        vpc = NULL;
    }

    return vpc;
}

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
 * CAUTION:
 * 本APIではAPIを呼び出した実プロセッサの番号が暗黙の引数として利用される。
 * APIを呼び出した実プロセッサと仮想プロセッサが関連付けされる。
 */
errno_t vm_launch(struct vm *vm, uint8_t vpc_no, const struct vpc_boot_configuration *boot)
{
    errno_t ret;
    struct vpc *vpc;

    assert(vm != NULL);

    vm_lock(vm);
    ret = validate_parameters(vm, vpc_no, boot);
    if (ret == SUCCESS) {
        vpc = activate_vpc(vm, vpc_no);
        vm_unlock(vm);
        if (vpc != NULL) {
            ret = vpc_launch(vpc, boot);
            if (ret == SUCCESS) {
                ret = vpc_event_loop(vpc);
            }
        } else {
            ret = -EBUSY;
        }
    } else {
        vm_unlock(vm);
    }

    return ret;
}

