/*
 * driver/aarch64/system_register/vtcr_el2.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_AARCH64_SYSTEM_REGISTER_VTCR_EL2_H
#define DRIVER_AARCH64_SYSTEM_REGISTER_VTCR_EL2_H

/* includes */

#include "lib/bit.h"

/* defines */

#define VTCR_EL2_PS_MASK    BITS(18, 16)
#define VTCR_EL2_PS_MSB     18
#define VTCR_EL2_PS_LSB     16
#define VTCR_EL2_PS(n)      (((n) << VTCR_EL2_PS_LSB) & VTCR_EL2_PS_MASK)
#define VTCR_EL2_TG0_MASK   BITS(15, 14)
#define VTCR_EL2_TG0_MSB    15
#define VTCR_EL2_TG0_LSB    14
#define VTCR_EL2_TG0(n)     (((n) << VTCR_EL2_TG0_LSB) & VTCR_EL2_TG0_MASK)
#define VTCR_EL2_SH0_MASK   BITS(13, 12)
#define VTCR_EL2_SH0_MSB    13
#define VTCR_EL2_SH0_LSB    12
#define VTCR_EL2_SH0(n)     (((n) << VTCR_EL2_SH0_LSB) & VTCR_EL2_SH0_MASK)
#define VTCR_EL2_ORGN0_MASK BITS(11, 10)
#define VTCR_EL2_ORGN0_MSB  11
#define VTCR_EL2_ORGN0_LSB  10
#define VTCR_EL2_ORGN0(n)   (((n) << VTCR_EL2_ORGN0_LSB) & VTCR_EL2_ORGN0_MASK)
#define VTCR_EL2_IRGN0_MASK BITS(9, 8)
#define VTCR_EL2_IRGN0_MSB  9
#define VTCR_EL2_IRGN0_LSB  8
#define VTCR_EL2_IRGN0(n)   (((n) << VTCR_EL2_IRGN0_LSB) & VTCR_EL2_IRGN0_MASK)
#define VTCR_EL2_SL0_MASK   BITS(7, 6)
#define VTCR_EL2_SL0_MSB    7
#define VTCR_EL2_SL0_LSB    6
#define VTCR_EL2_SL0(n)     (((n) << VTCR_EL2_SL0_LSB) & VTCR_EL2_SL0_MASK)
#define VTCR_EL2_T0SZ_MASK  BITS(5, 0)
#define VTCR_EL2_T0SZ_MSB   5
#define VTCR_EL2_T0SZ_LSB   0
#define VTCR_EL2_T0SZ(n)    (((n) << VTCR_EL2_T0SZ_LSB) & VTCR_EL2_T0SZ_MASK)
#define VTCR_EL2_RES1       (BIT(31))

#define VTCR_SH_NSH         0   /* Non-shareable */
#define VTCR_SH_OSH         2   /* Outer Shareable */
#define VTCR_SH_ISH         3   /* Inner Shareable */

#define VTCR_RGN_NC         0   /* Normal memory, Non-cacheable */
#define VTCR_RGN_WBWA       1   /* Normal memory, Write-Back Write-Allocate Cacheable */
#define VTCR_RGN_WT         2   /* Normal memory, Write-Through Cacheable */
#define VTCR_RGN_WB         3   /* Normal memory, Write-Back no Write-Allocate Cacheable */

#endif /* DRIVER_AARCH64_SYSTEM_REGISTER_VTCR_EL2_H */

