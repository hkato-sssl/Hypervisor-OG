/*
 * driver/arm/device/smmuv2/smmu_s2_cb.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef DRIVER_ARM_DEVICE_SMMUV2_SMMU_S2_CB_H
#define DRIVER_ARM_DEVICE_SMMUV2_SMMU_S2_CB_H

/* includes */

#include "lib/bit.h"

/* defines */

#define SMMU_S2_CB_SCTLR_WACFG_MASK             BITS(27, 26)
#define SMMU_S2_CB_SCTLR_WACFG_MSB              27
#define SMMU_S2_CB_SCTLR_WACFG_LSB              26
#define SMMU_S2_CB_SCTLR_WACFG(n)               (((n) << SMMU_S2_CB_SCTLR_WACFG_LSB) & SMMU_S2_CB_SCTLR_WACFG_MASK)
#define EXTRACT_SMMU_S2_CB_SCTLR_WACFG(d)       (((d) & SMMU_S2_CB_SCTLR_WACFG_MASK) >> SMMU_S2_CB_SCTLR_WACFG_LSB)
#define SMMU_S2_CB_SCTLR_RACFG_MASK             BITS(25, 24)
#define SMMU_S2_CB_SCTLR_RACFG_MSB              25
#define SMMU_S2_CB_SCTLR_RACFG_LSB              24
#define SMMU_S2_CB_SCTLR_RACFG(n)               (((n) << SMMU_S2_CB_SCTLR_RACFG_LSB) & SMMU_S2_CB_SCTLR_RACFG_MASK)
#define EXTRACT_SMMU_S2_CB_SCTLR_RACFG(d)       (((d) & SMMU_S2_CB_SCTLR_RACFG_MASK) >> SMMU_S2_CB_SCTLR_RACFG_LSB)
#define SMMU_S2_CB_SCTLR_SHCFG_MASK             BITS(23, 22)
#define SMMU_S2_CB_SCTLR_SHCFG_MSB              23
#define SMMU_S2_CB_SCTLR_SHCFG_LSB              22
#define SMMU_S2_CB_SCTLR_SHCFG(n)               (((n) << SMMU_S2_CB_SCTLR_SHCFG_LSB) & SMMU_S2_CB_SCTLR_SHCFG_MASK)
#define EXTRACT_SMMU_S2_CB_SCTLR_SHCFG(d)       (((d) & SMMU_S2_CB_SCTLR_SHCFG_MASK) >> SMMU_S2_CB_SCTLR_SHCFG_LSB)
#define SMMU_S2_CB_SCTLR_FB                     BIT(21)
#define SMMU_S2_CB_SCTLR_MemAttr_MASK           BITS(19, 16)
#define SMMU_S2_CB_SCTLR_MemAttr_MSB            19
#define SMMU_S2_CB_SCTLR_MemAttr_LSB            16
#define SMMU_S2_CB_SCTLR_MemAttr(n)             (((n) << SMMU_S2_CB_SCTLR_MemAttr_LSB) & SMMU_S2_CB_SCTLR_MemAttr_MASK)
#define EXTRACT_SMMU_S2_CB_SCTLR_MemAttr(d)     (((d) & SMMU_S2_CB_SCTLR_MemAttr_MASK) >> SMMU_S2_CB_SCTLR_MemAttr_LSB)
#define SMMU_S2_CB_SCTLR_BSU_MASK               BITS(15, 14)
#define SMMU_S2_CB_SCTLR_BSU_MSB                15
#define SMMU_S2_CB_SCTLR_BSU_LSB                14
#define SMMU_S2_CB_SCTLR_BSU(n)                 (((n) << SMMU_S2_CB_SCTLR_BSU_LSB) & SMMU_S2_CB_SCTLR_BSU_MASK)
#define EXTRACT_SMMU_S2_CB_SCTLR_BSU(d)         (((d) & SMMU_S2_CB_SCTLR_BSU_MASK) >> SMMU_S2_CB_SCTLR_BSU_LSB)
#define SMMU_S2_CB_SCTLR_PTW                    BIT(13)
#define SMMU_S2_CB_SCTLR_HUPCF                  BIT(8)
#define SMMU_S2_CB_SCTLR_CFCFG                  BIT(7)
#define SMMU_S2_CB_SCTLR_CFIE                   BIT(6)
#define SMMU_S2_CB_SCTLR_CFRE                   BIT(5)
#define SMMU_S2_CB_SCTLR_E                      BIT(4)
#define SMMU_S2_CB_SCTLR_AFFD                   BIT(3)
#define SMMU_S2_CB_SCTLR_AFE                    BIT(2)
#define SMMU_S2_CB_SCTLR_TRE                    BIT(1)
#define SMMU_S2_CB_SCTLR_M                      BIT(0)

#define SMMU_S2_CB_TCR_HD                       BIT(22)
#define SMMU_S2_CB_TCR_HA                       BIT(21)
#define SMMU_S2_CB_TCR_PASize_MASK              BITS(18, 16)
#define SMMU_S2_CB_TCR_PASize_MSB               18
#define SMMU_S2_CB_TCR_PASize_LSB               16
#define SMMU_S2_CB_TCR_PASize(n)                (((n) << SMMU_S2_CB_TCR_PASize_LSB) & SMMU_S2_CB_TCR_PASize_MASK)
#define EXTRACT_SMMU_S2_CB_TCR_PASize(d)        (((d) & SMMU_S2_CB_TCR_PASize_MASK) >> SMMU_S2_CB_TCR_PASize_LSB)
#define SMMU_S2_CB_TCR_TG0_MASK                 BITS(15, 14)
#define SMMU_S2_CB_TCR_TG0_MSB                  15
#define SMMU_S2_CB_TCR_TG0_LSB                  14
#define SMMU_S2_CB_TCR_TG0(n)                   (((n) << SMMU_S2_CB_TCR_TG0_LSB) & SMMU_S2_CB_TCR_TG0_MASK)
#define EXTRACT_SMMU_S2_CB_TCR_TG0(d)           (((d) & SMMU_S2_CB_TCR_TG0_MASK) >> SMMU_S2_CB_TCR_TG0_LSB)
#define SMMU_S2_CB_TCR_SH0_MASK                 BITS(13, 12)
#define SMMU_S2_CB_TCR_SH0_MSB                  13
#define SMMU_S2_CB_TCR_SH0_LSB                  12
#define SMMU_S2_CB_TCR_SH0(n)                   (((n) << SMMU_S2_CB_TCR_SH0_LSB) & SMMU_S2_CB_TCR_SH0_MASK)
#define EXTRACT_SMMU_S2_CB_TCR_SH0(d)           (((d) & SMMU_S2_CB_TCR_SH0_MASK) >> SMMU_S2_CB_TCR_SH0_LSB)
#define SMMU_S2_CB_TCR_ORGN0_MASK               BITS(11, 10)
#define SMMU_S2_CB_TCR_ORGN0_MSB                11
#define SMMU_S2_CB_TCR_ORGN0_LSB                10
#define SMMU_S2_CB_TCR_ORGN0(n)                 (((n) << SMMU_S2_CB_TCR_ORGN0_LSB) & SMMU_S2_CB_TCR_ORGN0_MASK)
#define EXTRACT_SMMU_S2_CB_TCR_ORGN0(d)         (((d) & SMMU_S2_CB_TCR_ORGN0_MASK) >> SMMU_S2_CB_TCR_ORGN0_LSB)
#define SMMU_S2_CB_TCR_IRGN0_MASK               BITS(9, 8)
#define SMMU_S2_CB_TCR_IRGN0_MSB                9
#define SMMU_S2_CB_TCR_IRGN0_LSB                8
#define SMMU_S2_CB_TCR_IRGN0(n)                 (((n) << SMMU_S2_CB_TCR_IRGN0_LSB) & SMMU_S2_CB_TCR_IRGN0_MASK)
#define EXTRACT_SMMU_S2_CB_TCR_IRGN0(d)         (((d) & SMMU_S2_CB_TCR_IRGN0_MASK) >> SMMU_S2_CB_TCR_IRGN0_LSB)
#define SMMU_S2_CB_TCR_SL0_MASK                 BITS(7, 6)
#define SMMU_S2_CB_TCR_SL0_MSB                  7
#define SMMU_S2_CB_TCR_SL0_LSB                  6
#define SMMU_S2_CB_TCR_SL0(n)                   (((n) << SMMU_S2_CB_TCR_SL0_LSB) & SMMU_S2_CB_TCR_SL0_MASK)
#define EXTRACT_SMMU_S2_CB_TCR_SL0(d)           (((d) & SMMU_S2_CB_TCR_SL0_MASK) >> SMMU_S2_CB_TCR_SL0_LSB)
#define SMMU_S2_CB_TCR_T0SZ_MASK                BITS(5, 0)
#define SMMU_S2_CB_TCR_T0SZ_MSB                 5
#define SMMU_S2_CB_TCR_T0SZ_LSB                 0
#define SMMU_S2_CB_TCR_T0SZ(n)                  (((n) << SMMU_S2_CB_TCR_T0SZ_LSB) & SMMU_S2_CB_TCR_T0SZ_MASK)
#define EXTRACT_SMMU_S2_CB_TCR_T0SZ(d)          (((d) & SMMU_S2_CB_TCR_T0SZ_MASK) >> SMMU_S2_CB_TCR_T0SZ_LSB)

#endif /* DRIVER_ARM_DEVICE_SMMUV2_SMMU_S2_CB_H */

