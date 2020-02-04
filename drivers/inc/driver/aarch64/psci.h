/*
 * driver/aarch64/psci.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_AARCH64_PSCI_H
#define DRIVER_AARCH64_PSCI_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>

/* defines */

#define PSCI_SMC64_PSCI_VERSION             0x84000000
#define PSCI_SMC64_CPU_SUSPEND              0xc4000001
#define PSCI_SMC64_CPU_OFF                  0x84000002
#define PSCI_SMC64_CPU_ON                   0xc4000003
#define PSCI_SMC64_AFFINITY_INFO            0xc4000004
#define PSCI_SMC64_MIGRATE                  0xc4000005
#define PSCI_SMC64_MIGRATE_INFO_TYPE        0x84000006
#define PSCI_SMC64_MIGRATE_INFO_UP_CPU      0xc4000007
#define PSCI_SMC64_SYSTEM_OFF               0x84000008
#define PSCI_SMC64_SYSTEM_RESET             0x84000009
#define PSCI_SMC64_PSCI_FEATURES            0x8400000a
#define PSCI_SMC64_CPU_FREEZE               0x8400000b
#define PSCI_SMC64_CPU_DEFAULT_SUSPEND      0xc400000c
#define PSCI_SMC64_NODE_HW_STATE            0xc400000d
#define PSCI_SMC64_SYSTEM_SUSPEND           0xc400000e
#define PSCI_SMC64_PSCI_SET_SUSPEND_MODE    0x8400000f
#define PSCI_SMC64_PSCI_STAT_RESIDENCY      0xc4000010
#define PSCI_SMC64_PSCI_STAT_COUNT          0xc4000011

/* types */

/* variables */

/* functions */

int32_t aarch64_psci_cpu_on(uint64_t target, void *entry, uintptr_t context);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_AARCH64_PSCI_H */

