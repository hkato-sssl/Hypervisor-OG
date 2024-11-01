// SPDX-License-Identifier: Apache-2.0
/*
 * driver/arm/device/smmuv2/smmu_cbar.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef DRIVER_ARM_DEVICE_SMMUV2_SMMU_CBAR_H
#define DRIVER_ARM_DEVICE_SMMUV2_SMMU_CBAR_H

/* includes */

#include "lib/bit.h"

/* defines */

#define SMMU_CBAR_S2_IRPTNDX_MASK BITS(31, 24)
#define SMMU_CBAR_S2_IRPTNDX_MSB  31
#define SMMU_CBAR_S2_IRPTNDX_LSB  24
#define SMMU_CBAR_S2_IRPTNDX(n) \
    (((n) << SMMU_CBAR_S2_IRPTNDX_LSB) & SMMU_CBAR_S2_IRPTNDX_MASK)
#define EXTRACT_SMMU_CBAR_S2_IRPTNDX(d) \
    (((d) & SMMU_CBAR_S2_IRPTNDX_MASK) >> SMMU_CBAR_S2_IRPTNDX_LSB)
#define SMMU_CBAR_S2_TYPE_MASK BITS(17, 16)
#define SMMU_CBAR_S2_TYPE_MSB  17
#define SMMU_CBAR_S2_TYPE_LSB  16
#define SMMU_CBAR_S2_TYPE(n) \
    (((n) << SMMU_CBAR_S2_TYPE_LSB) & SMMU_CBAR_S2_TYPE_MASK)
#define EXTRACT_SMMU_CBAR_S2_TYPE(d) \
    (((d) & SMMU_CBAR_S2_TYPE_MASK) >> SMMU_CBAR_S2_TYPE_LSB)
#define SMMU_CBAR_S2_VMID_MASK BITS(7, 0)
#define SMMU_CBAR_S2_VMID_MSB  7
#define SMMU_CBAR_S2_VMID_LSB  0
#define SMMU_CBAR_S2_VMID(n) \
    (((n) << SMMU_CBAR_S2_VMID_LSB) & SMMU_CBAR_S2_VMID_MASK)
#define EXTRACT_SMMU_CBAR_S2_VMID(d) \
    (((d) & SMMU_CBAR_S2_VMID_MASK) >> SMMU_CBAR_S2_VMID_LSB)

#endif /* DRIVER_ARM_DEVICE_SMMUV2_SMMU_CBAR_H */
