/*
 * emulator/psci/psci_emulate_aarch64.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/bit.h"
#include "driver/arm/psci_common.h"
#include "hypervisor/emulator/psci.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t unknown_function(struct vpc *vpc)
{
    errno_t ret;

    if (vpc->vm->emulator.psci.flag.report_unknown_function != 0) {
        ret = -ENOTSUP;
    } else {
        /*
         * Ref.: SMC Calling Convention (Issue B)
         *       5.2 Unknown Function Identifier
         */
        vpc->regs[VPC_X0] = BITS(63, 0);
        ret = SUCCESS;
    }
    
    return ret;
}

static errno_t call_op(struct vpc *vpc, vpc_exception_emulator_t op)
{
    errno_t ret;

    if (op != NULL) {
        ret = (*op)(vpc);
    } else {
        ret = unknown_function(vpc);
    }

    return ret;
}

static errno_t emulate_aarch64(struct vpc *vpc)
{
    errno_t ret;
    const struct psci_ops *ops;

    ops = vpc->vm->emulator.psci.aarch64.ops;
    switch (vpc->regs[VPC_X0] & BITS(31, 0)) {
    case PSCI_AARCH64_PSCI_VERSION:
        ret = call_op(vpc, ops->psci_version);
        break;
    case PSCI_AARCH64_CPU_SUSPEND:
        ret = call_op(vpc, ops->cpu_suspend);
        break;
    case PSCI_AARCH64_CPU_OFF:
        ret = call_op(vpc, ops->cpu_off);
        break;
    case PSCI_AARCH64_CPU_ON:
        ret = call_op(vpc, ops->cpu_on);
        break;
    case PSCI_AARCH64_AFFINITY_INFO:
        ret = call_op(vpc, ops->affinity_info);
        break;
    case PSCI_AARCH64_MIGRATE:
        ret = call_op(vpc, ops->migrate);
        break;
    case PSCI_AARCH64_MIGRATE_INFO_TYPE:
        ret = call_op(vpc, ops->migrate_info_type);
        break;
    case PSCI_AARCH64_MIGRATE_INFO_UP_CPU:
        ret = call_op(vpc, ops->migrate_info_up_cpu);
        break;
    case PSCI_AARCH64_SYSTEM_OFF:
        ret = call_op(vpc, ops->system_off);
        break;
    case PSCI_AARCH64_SYSTEM_RESET:
        ret = call_op(vpc, ops->system_reset);
        break;
    case PSCI_AARCH64_PSCI_FEATURES:
        ret = call_op(vpc, ops->psci_features);
        break;
    case PSCI_AARCH64_CPU_FREEZE:
        ret = call_op(vpc, ops->cpu_freeze);
        break;
    case PSCI_AARCH64_CPU_DEFAULT_SUSPEND:
        ret = call_op(vpc, ops->cpu_default_suspend);
        break;
    case PSCI_AARCH64_NODE_HW_STATE:
        ret = call_op(vpc, ops->node_hw_state);
        break;
    case PSCI_AARCH64_SYSTEM_SUSPEND:
        ret = call_op(vpc, ops->system_suspend);
        break;
    case PSCI_AARCH64_PSCI_SET_SUSPEND_MODE:
        ret = call_op(vpc, ops->psci_set_suspend_mode);
        break;
    case PSCI_AARCH64_PSCI_STAT_RESIDENCY:
        ret = call_op(vpc, ops->psci_stat_residency);
        break;
    case PSCI_AARCH64_PSCI_STAT_COUNT:
        ret = call_op(vpc, ops->psci_stat_count);
        break;
    default:
        ret = unknown_function(vpc);
        break;
    }

    return ret;
}

errno_t psci_emulate_aarch64(struct vpc *vpc)
{
    errno_t ret;

    if (vpc->vm->emulator.psci.aarch64.ops != NULL) {
        ret = emulate_aarch64(vpc);
    } else {
        ret = unknown_function(vpc);
    }

    return ret;
}

