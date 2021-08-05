/*
 * driver/xilinx/axi/device/uart_lite.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_XILINX_AXI_DEVICE_UART_LITE_H
#define DRIVER_XILINX_AXI_DEVICE_UART_LITE_H

/* includes */

#include "lib/bit.h"

/* defines */

#define UART_LITE_RX_FIFO  0x0000
#define UART_LITE_TX_FIFO  0x0004
#define UART_LITE_STAT_REG 0x0008
#define UART_LITE_CTRL_REG 0x000c

/* status register */

#define STAT_RX_FIFO_VALID BIT(0)
#define STAT_RX_FIFO_FULL  BIT(1)
#define STAT_TX_FIFO_EMPTY BIT(2)
#define STAT_TX_FIFO_FULL  BIT(3)
#define STAT_INTR_ENABLED  BIT(4)
#define STAT_OVERRUN_ERROR BIT(5)
#define STAT_FRAME_ERROR   BIT(6)
#define STAT_PARITY_ERROR  BIT(7)

/* control register */

#define CTRL_RST_TX_FIFO   BIT(0)
#define CTRL_RST_RX_FIFO   BIT(1)
#define CTRL_ENABLE_INTR   BIT(3)

#endif /* DRIVER_XILINX_AXI_DEVICE_UART_LITE_H */
