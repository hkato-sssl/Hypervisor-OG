/*
 * hypervisor/vm_initialize.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "driver/aarch64/stage2.h"
#include "hypervisor/vm.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t initialize(struct vm *vm, const struct vm_configuration *config)
{
    memset(vm, 0, sizeof(*vm));
    memset(&(vm->proc_map), VM_NO_ASSIGN, sizeof(vm->proc_map));
    spin_lock_init(&(vm->lock));
    vm->soc = config->soc;
    vm->nr_procs = config->nr_procs;
    vm->stage2 = config->stage2;

    return SUCCESS;
}

static errno_t validate_parameters(const struct vm *vm, const struct vm_configuration *config)
{
    errno_t ret;

    if (vm == NULL) {
        ret = -EINVAL;
    } else if (config == NULL) {
        ret = -EINVAL;
    } else if (config->nr_procs == 0) {
        ret = -EINVAL;
    } else if (config->nr_procs >= MAX_NR_VM_PROCESSORS) {
        ret = -EINVAL;
    } else if (config->stage2 == NULL) {
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

