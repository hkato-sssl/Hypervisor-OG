/*
 * driver/aarch64/system_register/cntv_ctl_el0.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef DRIVER_AARCH64_SYSTEM_REGISTER_CNTV_CTL_EL0_H
#define DRIVER_AARCH64_SYSTEM_REGISTER_CNTV_CTL_EL0_H

/* includes */

#include "lib/bit.h"

/* defines */

#define CNTV_CTL_EL0_ISTATUS BIT(2)
#define CNTV_CTL_EL0_IMASK   BIT(1)
#define CNTV_CTL_EL0_ENABLE  BIT(0)

#endif /* DRIVER_AARCH64_SYSTEM_REGISTER_CNTV_CTL_EL0_H */
