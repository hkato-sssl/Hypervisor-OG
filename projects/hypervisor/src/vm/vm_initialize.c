// SPDX-License-Identifier: Apache-2.0
/*
 * hypervisor/vm_initialize.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/aarch64/stage2.h"
#include "hypervisor/vm.h"
#include "lib/system/errno.h"
#include <stdint.h>
#include <string.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t initialize(struct vm *vm, const struct vm_configuration *config)
{
    errno_t ret;

    memset(vm, 0, sizeof(*vm));
    memset(&(vm->proc_map), VM_NO_ASSIGN, sizeof(vm->proc_map));

    spin_lock_init(&(vm->lock));
    vm->soc = config->soc;
    vm->nr_procs = config->nr_procs;

    ret = aarch64_stage2_initialize(&(vm->stage2), config->stage2);

    return ret;
}

static errno_t validate_parameters(const struct vm *vm,
                                   const struct vm_configuration *config)
{
    errno_t ret;

    if (config->nr_procs == 0) {
        ret = -EINVAL;
    } else if (config->nr_procs >= MAX_NR_VM_PROCESSORS) {
        ret = -EINVAL;
    } else {
        ret = SUCCESS;
    }

    return ret;
}

errno_t vm_initialize(struct vm *vm, const struct vm_configuration *config)
{
    errno_t ret;

    ret = validate_parameters(vm, config);
    if (ret == SUCCESS) {
        ret = initialize(vm, config);
    }

    return ret;
}
