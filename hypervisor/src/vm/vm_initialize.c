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
    vm->owner = config->owner;
    vm->nr_procs = config->nr_procs;
    vm->stage2 = config->stage2;

    return SUCCESS;
}

static bool is_valid_parameter(struct vm *vm, const struct vm_configuration *config)
{
    bool valid;

    if ((vm != NULL) && (config != NULL) && (config->nr_procs > 0) && (config->nr_procs <= MAX_NR_VM_PROCESSORS) && (config->stage2 != NULL)) {
        valid = true;
    } else {
        valid = false;
    }

    return valid;
}

errno_t vm_initialize(struct vm *vm, const struct vm_configuration *config)
{
    errno_t ret;

    if (is_valid_parameter(vm, config)) {
        ret = initialize(vm, config);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

