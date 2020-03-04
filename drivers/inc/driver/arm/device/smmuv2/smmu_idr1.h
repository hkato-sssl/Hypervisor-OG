/*
 * driver/arm/device/smmuv2/smmu_idr1.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef DRIVER_ARM_DEVICE_SMMUV2_SMMU_IDR1_H
#define DRIVER_ARM_DEVICE_SMMUV2_SMMU_IDR1_H

/* includes */

#include "lib/bit.h"

/* defines */

#define SMMU_IDR1_PAGESIZE                  BIT(31)
#define SMMU_IDR1_NUMPAGENDXB_MASK          BITS(30, 28)
#define SMMU_IDR1_NUMPAGENDXB_MSB           30
#define SMMU_IDR1_NUMPAGENDXB_LSB           28
#define SMMU_IDR1_NUMPAGENDXB(n)            (((n) << SMMU_IDR1_NUMPAGENDXB_LSB) & SMMU_IDR1_NUMPAGENDXB_MASK)
#define EXTRACT_SMMU_IDR1_NUMPAGENDXB(d)    (((d) & SMMU_IDR1_NUMPAGENDXB_MASK) >> SMMU_IDR1_NUMPAGENDXB_LSB)
#define SMMU_IDR1_HAFDBS_MASK               BITS(25, 24)
#define SMMU_IDR1_HAFDBS_MSB                25
#define SMMU_IDR1_HAFDBS_LSB                24
#define SMMU_IDR1_HAFDBS(n)                 (((n) << SMMU_IDR1_HAFDBS_LSB) & SMMU_IDR1_HAFDBS_MASK)
#define EXTRACT_SMMU_IDR1_HAFDBS(d)         (((d) & SMMU_IDR1_HAFDBS_MASK) >> SMMU_IDR1_HAFDBS_LSB)
#define SMMU_IDR1_NUMS2CB_MASK              BITS(23, 16)
#define SMMU_IDR1_NUMS2CB_MSB               23
#define SMMU_IDR1_NUMS2CB_LSB               16
#define SMMU_IDR1_NUMS2CB(n)                (((n) << SMMU_IDR1_NUMS2CB_LSB) & SMMU_IDR1_NUMS2CB_MASK)
#define EXTRACT_SMMU_IDR1_NUMS2CB(d)        (((d) & SMMU_IDR1_NUMS2CB_MASK) >> SMMU_IDR1_NUMS2CB_LSB)
#define SMMU_IDR1_SMCD                      BIT(15)
#define SMMU_IDR1_SSDTP_MASK                BITS(13, 12)
#define SMMU_IDR1_SSDTP_MSB                 13
#define SMMU_IDR1_SSDTP_LSB                 12
#define SMMU_IDR1_SSDTP(n)                  (((n) << SMMU_IDR1_SSDTP_LSB) & SMMU_IDR1_SSDTP_MASK)
#define EXTRACT_SMMU_IDR1_SSDTP(d)          (((d) & SMMU_IDR1_SSDTP_MASK) >> SMMU_IDR1_SSDTP_LSB)
#define SMMU_IDR1_NUMSSDNDXB_MASK           BITS(11, 8)
#define SMMU_IDR1_NUMSSDNDXB_MSB            11
#define SMMU_IDR1_NUMSSDNDXB_LSB            8
#define SMMU_IDR1_NUMSSDNDXB(n)             (((n) << SMMU_IDR1_NUMSSDNDXB_LSB) & SMMU_IDR1_NUMSSDNDXB_MASK)
#define EXTRACT_SMMU_IDR1_NUMSSDNDXB(d)     (((d) & SMMU_IDR1_NUMSSDNDXB_MASK) >> SMMU_IDR1_NUMSSDNDXB_LSB)
#define SMMU_IDR1_NUMCB_MASK                BITS(7, 0)
#define SMMU_IDR1_NUMCB_MSB                 7
#define SMMU_IDR1_NUMCB_LSB                 0
#define SMMU_IDR1_NUMCB(n)                  (((n) << SMMU_IDR1_NUMCB_LSB) & SMMU_IDR1_NUMCB_MASK)
#define EXTRACT_SMMU_IDR1_NUMCB(d)          (((d) & SMMU_IDR1_NUMCB_MASK) >> SMMU_IDR1_NUMCB_LSB)
#define SMMU_IDR1_RES1                      (BIT(14))

#endif /* DRIVER_ARM_DEVICE_SMMUV2_SMMU_IDR1_H */

