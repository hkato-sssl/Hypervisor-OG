/*
 * driver/arm/device/smmuv2/smmu_cr0.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef DRIVER_ARM_DEVICE_SMMUV2_SMMU_CR0_H
#define DRIVER_ARM_DEVICE_SMMUV2_SMMU_CR0_H

/* includes */

#include "lib/bit.h"

/* defines */

#define SMMU_CR0_VMID16EN                   BIT(31)
#define SMMU_CR0_HYPMODE                    BIT(30)
#define SMMU_CR0_WACFG_MASK                 BITS(27, 26)
#define SMMU_CR0_WACFG_MSB                  27
#define SMMU_CR0_WACFG_LSB                  26
#define SMMU_CR0_WACFG(n)                   (((n) << SMMU_CR0_WACFG_LSB) & SMMU_CR0_WACFG_MASK)
#define EXTRACT_SMMU_CR0_WACFG(d)           (((d) & SMMU_CR0_WACFG_MASK) >> SMMU_CR0_WACFG_LSB)
#define SMMU_CR0_RACFG_MASK                 BITS(25, 24)
#define SMMU_CR0_RACFG_MSB                  25
#define SMMU_CR0_RACFG_LSB                  24
#define SMMU_CR0_RACFG(n)                   (((n) << SMMU_CR0_RACFG_LSB) & SMMU_CR0_RACFG_MASK)
#define EXTRACT_SMMU_CR0_RACFG(d)           (((d) & SMMU_CR0_RACFG_MASK) >> SMMU_CR0_RACFG_LSB)
#define SMMU_CR0_SHCFG_MASK                 BITS(23, 22)
#define SMMU_CR0_SHCFG_MSB                  23
#define SMMU_CR0_SHCFG_LSB                  22
#define SMMU_CR0_SHCFG(n)                   (((n) << SMMU_CR0_SHCFG_LSB) & SMMU_CR0_SHCFG_MASK)
#define EXTRACT_SMMU_CR0_SHCFG(d)           (((d) & SMMU_CR0_SHCFG_MASK) >> SMMU_CR0_SHCFG_LSB)
#define SMMU_CR0_SMCFCFG                    BIT(21)
#define SMMU_CR0_MTCFG                      BIT(20)
#define SMMU_CR0_MEMATTR_MASK               BITS(19, 16)
#define SMMU_CR0_MEMATTR_MSB                19
#define SMMU_CR0_MEMATTR_LSB                16
#define SMMU_CR0_MEMATTR(n)                 (((n) << SMMU_CR0_MEMATTR_LSB) & SMMU_CR0_MEMATTR_MASK)
#define EXTRACT_SMMU_CR0_MEMATTR(d)         (((d) & SMMU_CR0_MEMATTR_MASK) >> SMMU_CR0_MEMATTR_LSB)
#define SMMU_CR0_BSU_MASK                   BITS(15, 14)
#define SMMU_CR0_BSU_MSB                    15
#define SMMU_CR0_BSU_LSB                    14
#define SMMU_CR0_BSU(n)                     (((n) << SMMU_CR0_BSU_LSB) & SMMU_CR0_BSU_MASK)
#define EXTRACT_SMMU_CR0_BSU(d)             (((d) & SMMU_CR0_BSU_MASK) >> SMMU_CR0_BSU_LSB)
#define SMMU_CR0_FB                         BIT(13)
#define SMMU_CR0_PTM                        BIT(12)
#define SMMU_CR0_VMIDPNE                    BIT(11)
#define SMMU_CR0_USFCFG                     BIT(10)
#define SMMU_CR0_GSE                        BIT(9)
#define SMMU_CR0_STALLD                     BIT(8)
#define SMMU_CR0_TRANSIENTCFG_MASK          BITS(7, 6)
#define SMMU_CR0_TRANSIENTCFG_MSB           7
#define SMMU_CR0_TRANSIENTCFG_LSB           6
#define SMMU_CR0_TRANSIENTCFG(n)            (((n) << SMMU_CR0_TRANSIENTCFG_LSB) & SMMU_CR0_TRANSIENTCFG_MASK)
#define EXTRACT_SMMU_CR0_TRANSIENTCFG(d)    (((d) & SMMU_CR0_TRANSIENTCFG_MASK) >> SMMU_CR0_TRANSIENTCFG_LSB)
#define SMMU_CR0_GCFGFIE                    BIT(5)
#define SMMU_CR0_GCFGFRE                    BIT(4)
#define SMMU_CR0_EXIDENABLE                 BIT(3)
#define SMMU_CR0_GFIE                       BIT(2)
#define SMMU_CR0_GFRE                       BIT(1)
#define SMMU_CR0_CLIENTPD                   BIT(0)

#endif /* DRIVER_ARM_DEVICE_SMMUV2_SMMU_CR0_H */

