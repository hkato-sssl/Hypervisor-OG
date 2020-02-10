/*
 * emulator/psci/psci_call_emulator_ops.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/arm/psci_common.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/psci.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t call_op(struct vpc *vpc, vpc_exception_emulator_t op)
{
    errno_t ret;

    if (op != NULL) {
        ret = (*op)(vpc);
    } else {
        ret = psci_error_unknown_function(vpc);
    }

    return ret;
}

static errno_t call_aarch64_emulator_ops(struct vpc *vpc, const struct psci_emulator_ops *ops)
{
    errno_t ret;
    uint32_t func;

    func = (uint32_t)(vpc->regs[VPC_X0]);
    switch (func) {
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
        ret = psci_error_unknown_function(vpc);
        break;
    }

    if (ret == SUCCESS) {
        ret = vpc_update_pc(vpc);
    }

    return ret;
}

static errno_t call_aarch32_emulator_ops(struct vpc *vpc, const struct psci_emulator_ops *ops)
{
    errno_t ret;
    uint32_t func;

    func = (uint32_t)(vpc->regs[VPC_X0]);
    switch (func) {
    case PSCI_AARCH32_PSCI_VERSION:
        ret = call_op(vpc, ops->psci_version);
        break;
    case PSCI_AARCH32_CPU_SUSPEND:
        ret = call_op(vpc, ops->cpu_suspend);
        break;
    case PSCI_AARCH32_CPU_OFF:
        ret = call_op(vpc, ops->cpu_off);
        break;
    case PSCI_AARCH32_CPU_ON:
        ret = call_op(vpc, ops->cpu_on);
        break;
    case PSCI_AARCH32_AFFINITY_INFO:
        ret = call_op(vpc, ops->affinity_info);
        break;
    case PSCI_AARCH32_MIGRATE:
        ret = call_op(vpc, ops->migrate);
        break;
    case PSCI_AARCH32_MIGRATE_INFO_TYPE:
        ret = call_op(vpc, ops->migrate_info_type);
        break;
    case PSCI_AARCH32_MIGRATE_INFO_UP_CPU:
        ret = call_op(vpc, ops->migrate_info_up_cpu);
        break;
    case PSCI_AARCH32_SYSTEM_OFF:
        ret = call_op(vpc, ops->system_off);
        break;
    case PSCI_AARCH32_SYSTEM_RESET:
        ret = call_op(vpc, ops->system_reset);
        break;
    case PSCI_AARCH32_PSCI_FEATURES:
        ret = call_op(vpc, ops->psci_features);
        break;
    case PSCI_AARCH32_CPU_FREEZE:
        ret = call_op(vpc, ops->cpu_freeze);
        break;
    case PSCI_AARCH32_CPU_DEFAULT_SUSPEND:
        ret = call_op(vpc, ops->cpu_default_suspend);
        break;
    case PSCI_AARCH32_NODE_HW_STATE:
        ret = call_op(vpc, ops->node_hw_state);
        break;
    case PSCI_AARCH32_SYSTEM_SUSPEND:
        ret = call_op(vpc, ops->system_suspend);
        break;
    case PSCI_AARCH32_PSCI_SET_SUSPEND_MODE:
        ret = call_op(vpc, ops->psci_set_suspend_mode);
        break;
    case PSCI_AARCH32_PSCI_STAT_RESIDENCY:
        ret = call_op(vpc, ops->psci_stat_residency);
        break;
    case PSCI_AARCH32_PSCI_STAT_COUNT:
        ret = call_op(vpc, ops->psci_stat_count);
        break;
    default:
        ret = psci_error_unknown_function(vpc);
        break;
    }

    if (ret == SUCCESS) {
        ret = vpc_update_pc(vpc);
    }

    return ret;
}

errno_t psci_call_emulator_ops(struct vpc *vpc, const struct psci_emulator_ops *ops)
{
    errno_t ret;

    if (vpc_is_aarch64(vpc)) {
        ret = call_aarch64_emulator_ops(vpc, ops);
    } else {
        ret = call_aarch32_emulator_ops(vpc, ops);
    }

    return ret;
}

errno_t psci_call_aarch64_emulator_ops(struct vpc *vpc, const struct psci_emulator_ops *ops)
{
    errno_t ret;

    if (vpc_is_aarch64(vpc)) {
        ret = call_aarch64_emulator_ops(vpc, ops);
    } else {
        ret = -EPERM;
    }

    return ret;
}

errno_t psci_call_aarch32_emulator_ops(struct vpc *vpc, const struct psci_emulator_ops *ops)
{
    errno_t ret;

    if (vpc_is_aarch32(vpc)) {
        ret = call_aarch32_emulator_ops(vpc, ops);
    } else {
        ret = -EPERM;
    }

    return ret;
}

