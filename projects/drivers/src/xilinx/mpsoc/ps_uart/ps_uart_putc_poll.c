// SPDX-License-Identifier: Apache-2.0
/*
 * xilinx/mpsoc/ps_uart/ps_uart_putc_poll.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/xilinx/mpsoc/device/ps_uart.h"
#include "driver/xilinx/mpsoc/ps_uart.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t ps_uart_putc_poll(uintptr_t reg_base, int ch)
{
    uint32_t d;

    do {
        d = REG_READ32(reg_base, PS_UART_CHANNEL_STS);
    } while ((d & BIT(3)) == 0);

    REG_WRITE32(reg_base, PS_UART_TX_RX_FIFO, ch);

    return SUCCESS;
}
