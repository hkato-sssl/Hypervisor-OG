/*
 * driver/aarch64/system_register/esr_el2_iss.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_AARCH64_SYSTEM_REGISTER_ESR_EL2_ISS_H
#define DRIVER_AARCH64_SYSTEM_REGISTER_ESR_EL2_ISS_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "lib/bit.h"

/* defines */

#define ISS_DATA_ABORT_ISV      BIT(24)
#define ISS_DATA_ABORT_SAS_MASK BITS(23, 22)
#define ISS_DATA_ABORT_SAS_MSB  23
#define ISS_DATA_ABORT_SAS_LSB  22
#define ISS_DATA_ABORT_SAS(n) \
    (((n) << ISS_DATA_ABORT_SAS_LSB) & ISS_DATA_ABORT_SAS_MASK)
#define EXTRACT_ISS_DATA_ABORT_SAS(d) \
    (((d)&ISS_DATA_ABORT_SAS_MASK) >> ISS_DATA_ABORT_SAS_LSB)
#define ISS_DATA_ABORT_SSE      BIT(21)
#define ISS_DATA_ABORT_SRT_MASK BITS(20, 16)
#define ISS_DATA_ABORT_SRT_MSB  20
#define ISS_DATA_ABORT_SRT_LSB  16
#define ISS_DATA_ABORT_SRT(n) \
    (((n) << ISS_DATA_ABORT_SRT_LSB) & ISS_DATA_ABORT_SRT_MASK)
#define EXTRACT_ISS_DATA_ABORT_SRT(d) \
    (((d)&ISS_DATA_ABORT_SRT_MASK) >> ISS_DATA_ABORT_SRT_LSB)
#define ISS_DATA_ABORT_SF        BIT(15)
#define ISS_DATA_ABORT_AR        BIT(14)
#define ISS_DATA_ABORT_FnV       BIT(10)
#define ISS_DATA_ABORT_EA        BIT(9)
#define ISS_DATA_ABORT_CM        BIT(8)
#define ISS_DATA_ABORT_S1PTW     BIT(7)
#define ISS_DATA_ABORT_WnR       BIT(6)
#define ISS_DATA_ABORT_DFSC_MASK BITS(5, 0)
#define ISS_DATA_ABORT_DFSC_MSB  5
#define ISS_DATA_ABORT_DFSC_LSB  0
#define ISS_DATA_ABORT_DFSC(n) \
    (((n) << ISS_DATA_ABORT_DFSC_LSB) & ISS_DATA_ABORT_DFSC_MASK)
#define EXTRACT_ISS_DATA_ABORT_DFSC(d) \
    (((d)&ISS_DATA_ABORT_DFSC_MASK) >> ISS_DATA_ABORT_DFSC_LSB)

/* types */

/* variables */

/* functions */

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_AARCH64_SYSTEM_REGISTER_ESR_EL2_ISS_H */
