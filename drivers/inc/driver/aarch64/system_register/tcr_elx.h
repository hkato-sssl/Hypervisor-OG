/*
 * driver/aarch64/system_register/tcr_elx.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_AARCH64_SYSTEM_REGISTER_TCR_ELX_H
#define DRIVER_AARCH64_SYSTEM_REGISTER_TCR_ELX_H

/* includes */

#include "lib/bit.h"

/* defines */

#define TCR_EL1_TBI1        BIT(38)
#define TCR_EL1_TBI0        BIT(37)
#define TCR_EL1_AS          BIT(36)
#define TCR_EL1_IPS_MASK    BITS(34, 32)
#define TCR_EL1_IPS_MSB     34
#define TCR_EL1_IPS_LSB     32
#define TCR_EL1_IPS(n)      (((n) << TCR_EL1_IPS_LSB) & TCR_EL1_IPS_MASK)
#define TCR_EL1_TG1_MASK    BITS(31, 30)
#define TCR_EL1_TG1_MSB     31
#define TCR_EL1_TG1_LSB     30
#define TCR_EL1_TG1(n)      (((n) << TCR_EL1_TG1_LSB) & TCR_EL1_TG1_MASK)
#define TCR_EL1_SH1_MASK    BITS(29, 28)
#define TCR_EL1_SH1_MSB     29
#define TCR_EL1_SH1_LSB     28
#define TCR_EL1_SH1(n)      (((n) << TCR_EL1_SH1_LSB) & TCR_EL1_SH1_MASK)
#define TCR_EL1_ORGN1_MASK  BITS(27, 26)
#define TCR_EL1_ORGN1_MSB   27
#define TCR_EL1_ORGN1_LSB   26
#define TCR_EL1_ORGN1(n)    (((n) << TCR_EL1_ORGN1_LSB) & TCR_EL1_ORGN1_MASK)
#define TCR_EL1_IRGN1_MASK  BITS(25, 24)
#define TCR_EL1_IRGN1_MSB   25
#define TCR_EL1_IRGN1_LSB   24
#define TCR_EL1_IRGN1(n)    (((n) << TCR_EL1_IRGN1_LSB) & TCR_EL1_IRGN1_MASK)
#define TCR_EL1_EPD1        BIT(23)
#define TCR_EL1_A1          BIT(22)
#define TCR_EL1_T1SZ_MASK   BITS(21, 16)
#define TCR_EL1_T1SZ_MSB    21
#define TCR_EL1_T1SZ_LSB    16
#define TCR_EL1_T1SZ(n)     (((n) << TCR_EL1_T1SZ_LSB) & TCR_EL1_T1SZ_MASK)
#define TCR_EL1_TG0_MASK    BITS(15, 14)
#define TCR_EL1_TG0_MSB     15
#define TCR_EL1_TG0_LSB     14
#define TCR_EL1_TG0(n)      (((n) << TCR_EL1_TG0_LSB) & TCR_EL1_TG0_MASK)
#define TCR_EL1_SH0_MASK    BITS(13, 12)
#define TCR_EL1_SH0_MSB     13
#define TCR_EL1_SH0_LSB     12
#define TCR_EL1_SH0(n)      (((n) << TCR_EL1_SH0_LSB) & TCR_EL1_SH0_MASK)
#define TCR_EL1_ORGN0_MASK  BITS(11, 10)
#define TCR_EL1_ORGN0_MSB   11
#define TCR_EL1_ORGN0_LSB   10
#define TCR_EL1_ORGN0(n)    (((n) << TCR_EL1_ORGN0_LSB) & TCR_EL1_ORGN0_MASK)
#define TCR_EL1_IRGN0_MASK  BITS(9, 8)
#define TCR_EL1_IRGN0_MSB   9
#define TCR_EL1_IRGN0_LSB   8
#define TCR_EL1_IRGN0(n)    (((n) << TCR_EL1_IRGN0_LSB) & TCR_EL1_IRGN0_MASK)
#define TCR_EL1_EPD0        BIT(7)
#define TCR_EL1_T0SZ_MASK   BITS(5, 0)
#define TCR_EL1_T0SZ_MSB    5
#define TCR_EL1_T0SZ_LSB    0
#define TCR_EL1_T0SZ(n)     (((n) << TCR_EL1_T0SZ_LSB) & TCR_EL1_T0SZ_MASK)

#define TCR_EL2_TBI         BIT(20)
#define TCR_EL2_PS_MASK     BITS(18, 16)
#define TCR_EL2_PS_MSB      18
#define TCR_EL2_PS_LSB      16
#define TCR_EL2_PS(n)       (((n) << TCR_EL2_PS_LSB) & TCR_EL2_PS_MASK)
#define TCR_EL2_TG0_MASK    BITS(15, 14)
#define TCR_EL2_TG0_MSB     15
#define TCR_EL2_TG0_LSB     14
#define TCR_EL2_TG0(n)      (((n) << TCR_EL2_TG0_LSB) & TCR_EL2_TG0_MASK)
#define TCR_EL2_SH0_MASK    BITS(13, 12)
#define TCR_EL2_SH0_MSB     13
#define TCR_EL2_SH0_LSB     12
#define TCR_EL2_SH0(n)      (((n) << TCR_EL2_SH0_LSB) & TCR_EL2_SH0_MASK)
#define TCR_EL2_ORGN0_MASK  BITS(11, 10)
#define TCR_EL2_ORGN0_MSB   11
#define TCR_EL2_ORGN0_LSB   10
#define TCR_EL2_ORGN0(n)    (((n) << TCR_EL2_ORGN0_LSB) & TCR_EL2_ORGN0_MASK)
#define TCR_EL2_IRGN0_MASK  BITS(9, 8)
#define TCR_EL2_IRGN0_MSB   9
#define TCR_EL2_IRGN0_LSB   8
#define TCR_EL2_IRGN0(n)    (((n) << TCR_EL2_IRGN0_LSB) & TCR_EL2_IRGN0_MASK)
#define TCR_EL2_T0SZ_MASK   BITS(5, 0)
#define TCR_EL2_T0SZ_MSB    5
#define TCR_EL2_T0SZ_LSB    0
#define TCR_EL2_T0SZ(n)     (((n) << TCR_EL2_T0SZ_LSB) & TCR_EL2_T0SZ_MASK)
#define TCR_EL2_RES1        (BIT(31) | BIT(23))

#define TCR_EL3_TBI         BIT(20)
#define TCR_EL3_PS_MASK     BITS(18, 16)
#define TCR_EL3_PS_MSB      18
#define TCR_EL3_PS_LSB      16
#define TCR_EL3_PS(n)       (((n) << TCR_EL3_PS_LSB) & TCR_EL3_PS_MASK)
#define TCR_EL3_TG0_MASK    BITS(15, 14)
#define TCR_EL3_TG0_MSB     15
#define TCR_EL3_TG0_LSB     14
#define TCR_EL3_TG0(n)      (((n) << TCR_EL3_TG0_LSB) & TCR_EL3_TG0_MASK)
#define TCR_EL3_SH0_MASK    BITS(13, 12)
#define TCR_EL3_SH0_MSB     13
#define TCR_EL3_SH0_LSB     12
#define TCR_EL3_SH0(n)      (((n) << TCR_EL3_SH0_LSB) & TCR_EL3_SH0_MASK)
#define TCR_EL3_ORGN0_MASK  BITS(11, 10)
#define TCR_EL3_ORGN0_MSB   11
#define TCR_EL3_ORGN0_LSB   10
#define TCR_EL3_ORGN0(n)    (((n) << TCR_EL3_ORGN0_LSB) & TCR_EL3_ORGN0_MASK)
#define TCR_EL3_IRGN0_MASK  BITS(9, 8)
#define TCR_EL3_IRGN0_MSB   9
#define TCR_EL3_IRGN0_LSB   8
#define TCR_EL3_IRGN0(n)    (((n) << TCR_EL3_IRGN0_LSB) & TCR_EL3_IRGN0_MASK)
#define TCR_EL3_T0SZ_MASK   BITS(5, 0)
#define TCR_EL3_T0SZ_MSB    5
#define TCR_EL3_T0SZ_LSB    0
#define TCR_EL3_T0SZ(n)     (((n) << TCR_EL3_T0SZ_LSB) & TCR_EL3_T0SZ_MASK)
#define TCR_EL3_RES1        (BIT(31) | BIT(23))

#define TCR_SH_NSH          0   /* Non-shareable */
#define TCR_SH_OSH          2   /* Outer Shareable */
#define TCR_SH_ISH          3   /* Inner Shareable */

#define TCR_RGN_NC          0   /* Normal memory, Non-cacheable */
#define TCR_RGN_WBWA        1   /* Normal memory, Write-Back Write-Allocate Cacheable */
#define TCR_RGN_WT          2   /* Normal memory, Write-Through Cacheable */
#define TCR_RGN_WB          3   /* Normal memory, Write-Back no Write-Allocate Cacheable */

#endif /* DRIVER_AARCH64_SYSTEM_REGISTER_TCR_ELX_H */

