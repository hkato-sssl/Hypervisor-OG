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

static errno_t configure_vpcs(struct vm *vm, struct vm_configuration const *config)
{
    errno_t ret;
    uint32_t i;
    struct vpc_configuration vpc_config;

    vpc_config.owner = vm;
    for (i = 0; i < config->nr_vpcs; ++i) {
        vpc_config.regs = config->vpc[i].regs;
        vpc_config.proc_no = (uint8_t)i;
        vpc_config.arch = VPC_AARCH64;
        vpc_config.gpr.pc = config->vpc[i].gpr.pc;
        vpc_config.gpr.sp = config->vpc[i].gpr.sp;
        ret = vpc_configure(&(vm->vpc[i]), &vpc_config);
        if (ret != SUCCESS) {
            break;
        }
    }

    return ret;
}

static errno_t configure(struct vm *vm, struct vm_configuration const *config)
{
    errno_t ret;

    memset(vm, 0, sizeof(*vm));
    vm->nr_vpcs = config->nr_vpcs;
    vm->stage2 = config->stage2;

    ret = configure_vpcs(vm, config);

    return ret;
}

static bool is_valid_parameter(struct vm *vm, struct vm_configuration const *config)
{
    bool valid;

    if ((vm != NULL) && (config != NULL) && (config->nr_vpcs > 0) && (config->stage2 != NULL)) {
        valid = true;
    } else {
        valid = false;
    }

    return valid;
}

errno_t vm_configure(struct vm *vm, struct vm_configuration const *config)
{
    errno_t ret;

    if (is_valid_parameter(vm, config)) {
        ret = configure(vm, config);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

