/*
 * arm/device/smmuv2/smmu_idr2.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef ARM_DEVICE_SMMUV2_SMMU_IDR2_H
#define ARM_DEVICE_SMMUV2_SMMU_IDR2_H

/* includes */

#include "lib/bit.h"

/* defines */

#define SMMU_IDR2_DIPANS         BIT(30)
#define SMMU_IDR2_HADS           BIT(28)
#define SMMU_IDR2_E2HS           BIT(27)
#define SMMU_IDR2_EXNUMSMRG_MASK BITS(26, 16)
#define SMMU_IDR2_EXNUMSMRG_MSB  26
#define SMMU_IDR2_EXNUMSMRG_LSB  16
#define SMMU_IDR2_EXNUMSMRG(n) \
    (((n) << SMMU_IDR2_EXNUMSMRG_LSB) & SMMU_IDR2_EXNUMSMRG_MASK)
#define EXTRACT_SMMU_IDR2_EXNUMSMRG(d) \
    (((d)&SMMU_IDR2_EXNUMSMRG_MASK) >> SMMU_IDR2_EXNUMSMRG_LSB)
#define SMMU_IDR2_VMID16S        BIT(15)
#define SMMU_IDR2_PTFSV8_64KB    BIT(14)
#define SMMU_IDR2_PTFSV8_16KB    BIT(13)
#define SMMU_IDR2_PTFSV8_4KB     BIT(12)
#define SMMU_IDR2_UBS_MASK       BITS(11, 8)
#define SMMU_IDR2_UBS_MSB        11
#define SMMU_IDR2_UBS_LSB        8
#define SMMU_IDR2_UBS(n)         (((n) << SMMU_IDR2_UBS_LSB) & SMMU_IDR2_UBS_MASK)
#define EXTRACT_SMMU_IDR2_UBS(d) (((d)&SMMU_IDR2_UBS_MASK) >> SMMU_IDR2_UBS_LSB)
#define SMMU_IDR2_OAS_MASK       BITS(7, 4)
#define SMMU_IDR2_OAS_MSB        7
#define SMMU_IDR2_OAS_LSB        4
#define SMMU_IDR2_OAS(n)         (((n) << SMMU_IDR2_OAS_LSB) & SMMU_IDR2_OAS_MASK)
#define EXTRACT_SMMU_IDR2_OAS(d) (((d)&SMMU_IDR2_OAS_MASK) >> SMMU_IDR2_OAS_LSB)
#define SMMU_IDR2_IAS_MASK       BITS(3, 0)
#define SMMU_IDR2_IAS_MSB        3
#define SMMU_IDR2_IAS_LSB        0
#define SMMU_IDR2_IAS(n)         (((n) << SMMU_IDR2_IAS_LSB) & SMMU_IDR2_IAS_MASK)
#define EXTRACT_SMMU_IDR2_IAS(d) (((d)&SMMU_IDR2_IAS_MASK) >> SMMU_IDR2_IAS_LSB)
#define SMMU_IDR2_RES1           (BIT(29))

#endif /* ARM_DEVICE_SMMUV2_SMMU_IDR2_H */
