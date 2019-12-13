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

    vpc_config.owner = vm;
    for (i = 0; i < config->nr_procs; ++i) {
        vpc_config.regs = config->regs.addr + (i * NR_VPC_REGS);
        vpc_config.proc_no = (uint8_t)i;
        ret = vpc_configure(vm_vpc(vm, i), &vpc_config);
        if (ret != SUCCESS) {
            break;
        }
    }

    return ret;
}

static errno_t configure(struct vm *vm, const struct vm_configuration *config)
{
    errno_t ret;

    memset(vm, 0, sizeof(*vm));
    spin_lock_init(&(vm->lock));
    vm->nr_procs = config->nr_procs;
    vm->vpcs = config->vpcs.addr;
    vm->stage2 = config->stage2;
    vm->boolean.launched = false;
    vm->boot = config->boot;

    ret = configure_vpcs(vm, config);

    return ret;
}

static bool is_valid_parameter(struct vm *vm, const struct vm_configuration *config)
{
    bool valid;

    if ((vm != NULL) && (config != NULL) && (config->nr_procs > 0) && (config->stage2 != NULL) &&
        ((config->vpcs.addr != NULL) && ((sizeof(struct vpc) * config->nr_procs) == config->vpcs.size)) &&
        ((config->regs.addr != NULL) && ((sizeof(uint64_t) * NR_VPC_REGS) == config->regs.size))) {
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

