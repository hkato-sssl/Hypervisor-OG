/*
 * hypervisor/vm_configure.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "driver/aarch64/stage2.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t configure_vpcs(struct vm *vm, const struct vm_configuration *config)
{
    errno_t ret;
    uint32_t i;
    struct vpc_configuration vpc_config;

    memset(&vpc_config, 0, sizeof(vpc_config));
    vpc_config.vm = vm;
    vpc_config.exception.ops = config->vpc.exception.ops;
    for (i = 0; i < config->nr_procs; ++i) {
        if (config->regs[i].size >= (NR_VPC_REGS * sizeof(uint64_t))) {
            vpc_config.regs = config->regs[i].addr;
            vpc_config.proc_no = (uint8_t)i;
            ret = vpc_configure(vm_vpc(vm, i), &vpc_config);
            if (ret != SUCCESS) {
                break;
            }
        } else {
            ret = -EINVAL;
            break;
        }
    }

    return ret;
}

static errno_t configure(struct vm *vm, const struct vm_configuration *config)
{
    errno_t ret;
    int i;

    memset(vm, 0, sizeof(*vm));
    memset(&(vm->proc_map), VM_NO_ASSIGN, sizeof(vm->proc_map));
    spin_lock_init(&(vm->lock));
    vm->owner = config->owner;
    vm->nr_procs = config->nr_procs;

    for (i = 0; i < vm->nr_procs; ++i) {
        vm->vpcs[i] = config->vpcs[i];
    }

    vm->stage2 = config->stage2;
    vm->boolean.launched = false;

    ret = configure_vpcs(vm, config);

    return ret;
}

static bool is_valid_parameter(struct vm *vm, const struct vm_configuration *config)
{
    bool valid;

    if ((vm != NULL) && (config != NULL) && (config->nr_procs > 0) && (config->nr_procs <= MAX_NR_VM_PROCESSORS) && (config->stage2 != NULL) &&
        (config->vpcs != NULL)) {
        valid = true;
    } else {
        valid = false;
    }

    return valid;
}

errno_t vm_configure(struct vm *vm, const struct vm_configuration *config)
{
    errno_t ret;

    if (is_valid_parameter(vm, config)) {
        ret = configure(vm, config);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

