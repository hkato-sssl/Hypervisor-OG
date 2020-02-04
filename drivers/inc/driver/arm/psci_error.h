/*
 * driver/arm/psci.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef DRIVER_ARM_PSCI_ERROR_H
#define DRIVER_ARM_PSCI_ERROR_H

/* includes */

/* defines */

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

#endif /* DRIVER_ARM_PSCI_ERROR_H */

