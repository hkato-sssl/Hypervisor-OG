/*
 * hypervisor/emulator/psci.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef HYPERVISOR_EMULATOR_PSCI_H
#define HYPERVISOR_EMULATOR_PSCI_H

/* includes */

#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "lib/system/errno.h"
#include <stdint.h>

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* defines */

/* types */

struct psci_emulator_ops {
    vpc_exception_emulator_t psci_version;
    vpc_exception_emulator_t cpu_suspend;
    vpc_exception_emulator_t cpu_off;
    vpc_exception_emulator_t cpu_on;
    vpc_exception_emulator_t affinity_info;
    vpc_exception_emulator_t migrate;
    vpc_exception_emulator_t migrate_info_type;
    vpc_exception_emulator_t migrate_info_up_cpu;
    vpc_exception_emulator_t system_off;
    vpc_exception_emulator_t system_reset;
    vpc_exception_emulator_t psci_features;
    vpc_exception_emulator_t cpu_freeze;
    vpc_exception_emulator_t cpu_default_suspend;
    vpc_exception_emulator_t node_hw_state;
    vpc_exception_emulator_t system_suspend;
    vpc_exception_emulator_t psci_set_suspend_mode;
    vpc_exception_emulator_t psci_stat_residency;
    vpc_exception_emulator_t psci_stat_count;
};

/* variables */

/* functions */

errno_t psci_call_emulator_ops(struct vpc *vpc,
                               const struct psci_emulator_ops *ops);
errno_t psci_call_aarch64_emulator_ops(struct vpc *vpc,
                                       const struct psci_emulator_ops *ops);
errno_t psci_call_aarch32_emulator_ops(struct vpc *vpc,
                                       const struct psci_emulator_ops *ops);
errno_t psci_set_error(struct vpc *vpc, int64_t error);
errno_t psci_error_unknown_function(struct vpc *vpc);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_EMULATOR_PSCI_H */
