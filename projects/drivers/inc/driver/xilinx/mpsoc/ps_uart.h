// SPDX-License-Identifier: Apache-2.0
/*
 * driver/xilinx/mpsoc/ps_uart.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_XILINX_MPSOC_PS_UART_H
#define DRIVER_XILINX_MPSOC_PS_UART_H

/* includes */

/* defines */

#ifndef ASSEMBLY

/* includes */

#include "lib/system/errno.h"
#include <stdint.h>

/* defines */

/* types */

/* variables */

/* functions */

#ifdef __cplusplus
extern "C" {
#endif

errno_t ps_uart_putc_poll(uintptr_t reg_base, int ch);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_XILINX_MPSOC_PS_UART_H */
