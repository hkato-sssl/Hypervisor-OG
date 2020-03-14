/*
 * driver/arm/device/smmuv2/smmu_idr0.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef DRIVER_ARM_DEVICE_SMMUV2_SMMU_IDR0_H
#define DRIVER_ARM_DEVICE_SMMUV2_SMMU_IDR0_H

/* includes */

#include "lib/bit.h"

/* defines */

#define SMMU_IDR0_SES                   BIT(31)
#define SMMU_IDR0_S1TS                  BIT(30)
#define SMMU_IDR0_S2TS                  BIT(29)
#define SMMU_IDR0_NTS                   BIT(28)
#define SMMU_IDR0_SMS                   BIT(27)
#define SMMU_IDR0_ATOSNS                BIT(26)
#define SMMU_IDR0_PTFS_MASK             BITS(25, 24)
#define SMMU_IDR0_PTFS_MSB              25
#define SMMU_IDR0_PTFS_LSB              24
#define SMMU_IDR0_PTFS(n)               (((n) << SMMU_IDR0_PTFS_LSB) & SMMU_IDR0_PTFS_MASK)
#define EXTRACT_SMMU_IDR0_PTFS(d)       (((d) & SMMU_IDR0_PTFS_MASK) >> SMMU_IDR0_PTFS_LSB)
#define SMMU_IDR0_NUMIRPT_MASK          BITS(23, 16)
#define SMMU_IDR0_NUMIRPT_MSB           23
#define SMMU_IDR0_NUMIRPT_LSB           16
#define SMMU_IDR0_NUMIRPT(n)            (((n) << SMMU_IDR0_NUMIRPT_LSB) & SMMU_IDR0_NUMIRPT_MASK)
#define EXTRACT_SMMU_IDR0_NUMIRPT(d)    (((d) & SMMU_IDR0_NUMIRPT_MASK) >> SMMU_IDR0_NUMIRPT_LSB)
#define SMMU_IDR0_EXSMRGS               BIT(15)
#define SMMU_IDR0_CTTW                  BIT(14)
#define SMMU_IDR0_BTM                   BIT(13)
#define SMMU_IDR0_NUMSIDB_MASK          BITS(12, 9)
#define SMMU_IDR0_NUMSIDB_MSB           12
#define SMMU_IDR0_NUMSIDB_LSB           9
#define SMMU_IDR0_NUMSIDB(n)            (((n) << SMMU_IDR0_NUMSIDB_LSB) & SMMU_IDR0_NUMSIDB_MASK)
#define EXTRACT_SMMU_IDR0_NUMSIDB(d)    (((d) & SMMU_IDR0_NUMSIDB_MASK) >> SMMU_IDR0_NUMSIDB_LSB)
#define SMMU_IDR0_EXIDS                 BIT(8)
#define SMMU_IDR0_NUMSMRG_MASK          BITS(7, 0)
#define SMMU_IDR0_NUMSMRG_MSB           7
#define SMMU_IDR0_NUMSMRG_LSB           0
#define SMMU_IDR0_NUMSMRG(n)            (((n) << SMMU_IDR0_NUMSMRG_LSB) & SMMU_IDR0_NUMSMRG_MASK)
#define EXTRACT_SMMU_IDR0_NUMSMRG(d)    (((d) & SMMU_IDR0_NUMSMRG_MASK) >> SMMU_IDR0_NUMSMRG_LSB)

#endif /* DRIVER_ARM_DEVICE_SMMUV2_SMMU_IDR0_H */
