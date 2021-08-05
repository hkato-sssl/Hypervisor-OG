/*
 * driver/aarch64/system_register/hcr_el2.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_AARCH64_SYSTEM_REGISTER_HCR_EL2_H
#define DRIVER_AARCH64_SYSTEM_REGISTER_HCR_EL2_H

/* includes */

#include "lib/bit.h"

/* defines */

#define HCR_EL2_MIOCNCE  BIT(38)
#define HCR_EL2_ID       BIT(33)
#define HCR_EL2_CD       BIT(32)
#define HCR_EL2_RW       BIT(31)
#define HCR_EL2_TRVM     BIT(30)
#define HCR_EL2_HCD      BIT(29)
#define HCR_EL2_TDZ      BIT(28)
#define HCR_EL2_TGE      BIT(27)
#define HCR_EL2_TVM      BIT(26)
#define HCR_EL2_TTLB     BIT(25)
#define HCR_EL2_TPU      BIT(24)
#define HCR_EL2_TPC      BIT(23)
#define HCR_EL2_TSW      BIT(22)
#define HCR_EL2_TACR     BIT(21)
#define HCR_EL2_TIDCP    BIT(20)
#define HCR_EL2_TSC      BIT(19)
#define HCR_EL2_TID3     BIT(18)
#define HCR_EL2_TID2     BIT(17)
#define HCR_EL2_TID1     BIT(16)
#define HCR_EL2_TID0     BIT(15)
#define HCR_EL2_TWE      BIT(14)
#define HCR_EL2_TWI      BIT(13)
#define HCR_EL2_DC       BIT(12)
#define HCR_EL2_BSU_MASK BITS(11, 10)
#define HCR_EL2_BSU_MSB  11
#define HCR_EL2_BSU_LSB  10
#define HCR_EL2_BSU(n)   (((n) << HCR_EL2_BSU_LSB) & HCR_EL2_BSU_MASK)
#define HCR_EL2_FB       BIT(9)
#define HCR_EL2_VSE      BIT(8)
#define HCR_EL2_VI       BIT(7)
#define HCR_EL2_VF       BIT(6)
#define HCR_EL2_AMO      BIT(5)
#define HCR_EL2_IMO      BIT(4)
#define HCR_EL2_FMO      BIT(3)
#define HCR_EL2_PTW      BIT(2)
#define HCR_EL2_SWIO     BIT(1)
#define HCR_EL2_VM       BIT(0)

#endif /* DRIVER_AARCH64_SYSTEM_REGISTER_HCR_EL2_H */
