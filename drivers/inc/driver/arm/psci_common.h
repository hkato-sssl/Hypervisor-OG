/*
 * driver/arm/psci.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef DRIVER_ARM_PSCI_COMMON_H
#define DRIVER_ARM_PSCI_COMMON_H

/* includes */

/* defines */

/* error codes */

#define PSCI_SUCCESS                    0
#define PSCI_ERROR_NOT_SUPPORTED        (-1)
#define PSCI_ERROR_INVALID_PARAMETERS   (-2)
#define PSCI_ERROR_DENIED               (-3)
#define PSCI_ERROR_ALREADY_ON           (-4)
#define PSCI_ERROR_ON_PENDING           (-5)
#define PSCI_ERROR_INTERNAL_FAILURE     (-6)
#define PSCI_ERROR_NOT_PRESENT          (-7)
#define PSCI_ERROR_DISABLED             (-8)
#define PSCI_ERROR_INVALID_ADDRESS      (-9)

/* function ID for AArch64 */

#define PSCI_AARCH64_PSCI_VERSION             0x84000000
#define PSCI_AARCH64_CPU_SUSPEND              0xc4000001
#define PSCI_AARCH64_CPU_OFF                  0x84000002
#define PSCI_AARCH64_CPU_ON                   0xc4000003
#define PSCI_AARCH64_AFFINITY_INFO            0xc4000004
#define PSCI_AARCH64_MIGRATE                  0xc4000005
#define PSCI_AARCH64_MIGRATE_INFO_TYPE        0x84000006
#define PSCI_AARCH64_MIGRATE_INFO_UP_CPU      0xc4000007
#define PSCI_AARCH64_SYSTEM_OFF               0x84000008
#define PSCI_AARCH64_SYSTEM_RESET             0x84000009
#define PSCI_AARCH64_PSCI_FEATURES            0x8400000a
#define PSCI_AARCH64_CPU_FREEZE               0x8400000b
#define PSCI_AARCH64_CPU_DEFAULT_SUSPEND      0xc400000c
#define PSCI_AARCH64_NODE_HW_STATE            0xc400000d
#define PSCI_AARCH64_SYSTEM_SUSPEND           0xc400000e
#define PSCI_AARCH64_PSCI_SET_SUSPEND_MODE    0x8400000f
#define PSCI_AARCH64_PSCI_STAT_RESIDENCY      0xc4000010
#define PSCI_AARCH64_PSCI_STAT_COUNT          0xc4000011

#endif /* DRIVER_ARM_PSCI_COMMON_H */

