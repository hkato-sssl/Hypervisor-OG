/*
 * emulator/psci/simple/simple_psci_cpu_on.c
 *
 * (C) 2020 Hidekazu Kato
 */

/* NOTE:
 * 本APIは一番単純なCPU_ON機能のエミュレータとなる。
 * マルチコア関連の起動・停止処理の実装にはOS層の機能を利用する事が想定
 * される為、実装例という形で提供される。
 */
 
#include <stdint.h>
#include <string.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "driver/arm/psci_common.h"
#include "hypervisor/soc.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/psci.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t cpu_on(struct vpc *vpc, uint16_t target, uint64_t addr)
{
    errno_t ret;
    struct vpc_boot_configuration boot;

    memset(&boot, 0, sizeof(boot));
    boot.arch = VPC_ARCH_AARCH64;
    boot.pc = addr;
    boot.arg = vpc->regs[VPC_X3];

    ret = vm_wakeup(vpc->vm, target, &boot);
    if (ret == SUCCESS) {
        ret = psci_set_error(vpc, PSCI_SUCCESS);
    } else if (ret == -EBUSY) {
        ret = psci_set_error(vpc, PSCI_ERROR_ALREADY_ON);
    } else if (ret == -EINVAL) {
        ret = psci_set_error(vpc, PSCI_ERROR_INVALID_PARAMETERS);
    } else {
        ret = psci_set_error(vpc, PSCI_ERROR_INTERNAL_FAILURE);
    }

    return ret;
}

errno_t simple_psci_cpu_on(struct vpc *vpc)
{
    errno_t ret;
    uint64_t target;
    uint64_t addr;

    /*
     * NOTE: Aff3, Aff2 and Aff1 must be zero.
     */

    target = vpc->regs[VPC_X1];
    addr = vpc->regs[VPC_X2];

    if (target >= vpc->vm->nr_procs) {
        ret = psci_set_error(vpc, PSCI_ERROR_INVALID_PARAMETERS);
    } else if ((! IS_ALIGNED(addr, 4)) || (! soc_test_executable_region(vpc->vm->soc, addr))) {
        ret = psci_set_error(vpc, PSCI_ERROR_INVALID_ADDRESS);
    } else {
        ret = cpu_on(vpc, target, addr);
    }

    return ret;
}

