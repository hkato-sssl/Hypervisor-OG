/*
 * driver/arm/device/smmuv2/smmu_s2cr.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef DRIVER_ARM_DEVICE_SMMUV2_SMMU_S2CR_H
#define DRIVER_ARM_DEVICE_SMMUV2_SMMU_S2CR_H

/* includes */

#include "lib/bit.h"

/* defines */

#define SMMU_S2CR_TRANSIENTCFG_MASK         BITS(29, 28)
#define SMMU_S2CR_TRANSIENTCFG_MSB          29
#define SMMU_S2CR_TRANSIENTCFG_LSB          28
#define SMMU_S2CR_TRANSIENTCFG(n)           (((n) << SMMU_S2CR_TRANSIENTCFG_LSB) & SMMU_S2CR_TRANSIENTCFG_MASK)
#define EXTRACT_SMMU_S2CR_TRANSIENTCFG(d)   (((d) & SMMU_S2CR_TRANSIENTCFG_MASK) >> SMMU_S2CR_TRANSIENTCFG_LSB)
#define SMMU_S2CR_INSTCFG_MASK              BITS(27, 26)
#define SMMU_S2CR_INSTCFG_MSB               27
#define SMMU_S2CR_INSTCFG_LSB               26
#define SMMU_S2CR_INSTCFG(n)                (((n) << SMMU_S2CR_INSTCFG_LSB) & SMMU_S2CR_INSTCFG_MASK)
#define EXTRACT_SMMU_S2CR_INSTCFG(d)        (((d) & SMMU_S2CR_INSTCFG_MASK) >> SMMU_S2CR_INSTCFG_LSB)
#define SMMU_S2CR_PRIVCFG_MASK              BITS(25, 24)
#define SMMU_S2CR_PRIVCFG_MSB               25
#define SMMU_S2CR_PRIVCFG_LSB               24
#define SMMU_S2CR_PRIVCFG(n)                (((n) << SMMU_S2CR_PRIVCFG_LSB) & SMMU_S2CR_PRIVCFG_MASK)
#define EXTRACT_SMMU_S2CR_PRIVCFG(d)        (((d) & SMMU_S2CR_PRIVCFG_MASK) >> SMMU_S2CR_PRIVCFG_LSB)
#define SMMU_S2CR_WACFG_MASK                BITS(23, 22)
#define SMMU_S2CR_WACFG_MSB                 23
#define SMMU_S2CR_WACFG_LSB                 22
#define SMMU_S2CR_WACFG(n)                  (((n) << SMMU_S2CR_WACFG_LSB) & SMMU_S2CR_WACFG_MASK)
#define EXTRACT_SMMU_S2CR_WACFG(d)          (((d) & SMMU_S2CR_WACFG_MASK) >> SMMU_S2CR_WACFG_LSB)
#define SMMU_S2CR_RACFG_MASK                BITS(21, 20)
#define SMMU_S2CR_RACFG_MSB                 21
#define SMMU_S2CR_RACFG_LSB                 20
#define SMMU_S2CR_RACFG(n)                  (((n) << SMMU_S2CR_RACFG_LSB) & SMMU_S2CR_RACFG_MASK)
#define EXTRACT_SMMU_S2CR_RACFG(d)          (((d) & SMMU_S2CR_RACFG_MASK) >> SMMU_S2CR_RACFG_LSB)
#define SMMU_S2CR_NSCFG_MASK                BITS(19, 18)
#define SMMU_S2CR_NSCFG_MSB                 19
#define SMMU_S2CR_NSCFG_LSB                 18
#define SMMU_S2CR_NSCFG(n)                  (((n) << SMMU_S2CR_NSCFG_LSB) & SMMU_S2CR_NSCFG_MASK)
#define EXTRACT_SMMU_S2CR_NSCFG(d)          (((d) & SMMU_S2CR_NSCFG_MASK) >> SMMU_S2CR_NSCFG_LSB)
#define SMMU_S2CR_TYPE_MASK                 BITS(17, 16)
#define SMMU_S2CR_TYPE_MSB                  17
#define SMMU_S2CR_TYPE_LSB                  16
#define SMMU_S2CR_TYPE(n)                   (((n) << SMMU_S2CR_TYPE_LSB) & SMMU_S2CR_TYPE_MASK)
#define EXTRACT_SMMU_S2CR_TYPE(d)           (((d) & SMMU_S2CR_TYPE_MASK) >> SMMU_S2CR_TYPE_LSB)
#define SMMU_S2CR_MEMATTR_MASK              BITS(15, 12)
#define SMMU_S2CR_MEMATTR_MSB               15
#define SMMU_S2CR_MEMATTR_LSB               12
#define SMMU_S2CR_MEMATTR(n)                (((n) << SMMU_S2CR_MEMATTR_LSB) & SMMU_S2CR_MEMATTR_MASK)
#define EXTRACT_SMMU_S2CR_MEMATTR(d)        (((d) & SMMU_S2CR_MEMATTR_MASK) >> SMMU_S2CR_MEMATTR_LSB)
#define SMMU_S2CR_MTCFG                     BIT(11)
#define SMMU_S2CR_EXIDVALID                 BIT(10)
#define SMMU_S2CR_SHCFG_MASK                BITS(9, 8)
#define SMMU_S2CR_SHCFG_MSB                 9
#define SMMU_S2CR_SHCFG_LSB                 8
#define SMMU_S2CR_SHCFG(n)                  (((n) << SMMU_S2CR_SHCFG_LSB) & SMMU_S2CR_SHCFG_MASK)
#define EXTRACT_SMMU_S2CR_SHCFG(d)          (((d) & SMMU_S2CR_SHCFG_MASK) >> SMMU_S2CR_SHCFG_LSB)
#define SMMU_S2CR_CBNDX_MASK                BITS(7, 0)
#define SMMU_S2CR_CBNDX_MSB                 7
#define SMMU_S2CR_CBNDX_LSB                 0
#define SMMU_S2CR_CBNDX(n)                  (((n) << SMMU_S2CR_CBNDX_LSB) & SMMU_S2CR_CBNDX_MASK)
#define EXTRACT_SMMU_S2CR_CBNDX(d)          (((d) & SMMU_S2CR_CBNDX_MASK) >> SMMU_S2CR_CBNDX_LSB)

#endif /* DRIVER_ARM_DEVICE_SMMUV2_SMMU_S2CR_H */

