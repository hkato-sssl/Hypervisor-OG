/*
 * driver/arm/device/smmuv2/smmu_cba2r.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef DRIVER_ARM_DEVICE_SMMUV2_SMMU_CBA2R_H
#define DRIVER_ARM_DEVICE_SMMUV2_SMMU_CBA2R_H

/* includes */

#include "lib/bit.h"

/* defines */

#define SMMU_CBA2R_VMID16_MASK              BITS(31, 16)
#define SMMU_CBA2R_VMID16_MSB               31
#define SMMU_CBA2R_VMID16_LSB               16
#define SMMU_CBA2R_VMID16(n)                (((n) << SMMU_CBA2R_VMID16_LSB) & SMMU_CBA2R_VMID16_MASK)
#define EXTRACT_SMMU_CBA2R_VMID16(d)        (((d) & SMMU_CBA2R_VMID16_MASK) >> SMMU_CBA2R_VMID16_LSB)
#define SMMU_CBA2R_MONC                     BIT(1)
#define SMMU_CBA2R_VA64                     BIT(0)

#endif /* DRIVER_ARM_DEVICE_SMMUV2_SMMU_CBA2R_H */

