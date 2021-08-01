/*
 * driver/aarch64/system_register/esr_elx.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef DRIVER_AARCH64_SYSTEM_REGISTER_ESR_ELX_H
#define DRIVER_AARCH64_SYSTEM_REGISTER_ESR_ELX_H

/* includes */

#include "lib/bit.h"

/* defines */

#define ESR_EC_MASK                 BITS(31, 26)
#define ESR_EC_MSB                  31
#define ESR_EC_LSB                  26
#define ESR_EC(n)                   (((n) << ESR_EC_LSB) & ESR_EC_MASK)
#define EXTRACT_ESR_EC(d)           (((d) & ESR_EC_MASK) >> ESR_EC_LSB)
#define ESR_IL                      BIT(25)
#define ESR_ISS_MASK                BITS(24, 0)
#define ESR_ISS_MSB                 24
#define ESR_ISS_LSB                 0
#define ESR_ISS(n)                  (((n) << ESR_ISS_LSB) & ESR_ISS_MASK)
#define EXTRACT_ESR_ISS(d)          (((d) & ESR_ISS_MASK) >> ESR_ISS_LSB)

#endif /* DRIVER_AARCH64_SYSTEM_REGISTER_ESR_ELX_H */

