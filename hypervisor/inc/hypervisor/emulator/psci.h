/*
 * hypervisor/emulator/psci.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef HYPERVISOR_EMULATOR_PSCI_H
#define HYPERVISOR_EMULATOR_PSCI_H

/* includes */

#include <stdint.h>
#include "lib/system/errno.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* defines */

/* types */

struct psci_ops {
    vpc_exception_emulator_t    psci_version;
    vpc_exception_emulator_t    cpu_suspend;
    vpc_exception_emulator_t    cpu_off;
    vpc_exception_emulator_t    cpu_on;
    vpc_exception_emulator_t    affinity_info;
    vpc_exception_emulator_t    migrate;
    vpc_exception_emulator_t    migrate_info_type;
    vpc_exception_emulator_t    migrate_info_up_cpu;
    vpc_exception_emulator_t    system_off;
    vpc_exception_emulator_t    system_reset;
    vpc_exception_emulator_t    psci_features;
    vpc_exception_emulator_t    cpu_freeze;
    vpc_exception_emulator_t    cpu_default_suspend;
    vpc_exception_emulator_t    node_hw_state;
    vpc_exception_emulator_t    system_suspend;
    vpc_exception_emulator_t    psci_set_suspend_mode;
    vpc_exception_emulator_t    psci_stat_residency;
    vpc_exception_emulator_t    psci_stat_count;
};

/* variables */

/* functions */

errno_t psci_emulate_aarch64_request(struct vpc *vpc);
errno_t psci_set_error(struct vpc *vpc, int64_t error);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_EMULATOR_PSCI_H */

