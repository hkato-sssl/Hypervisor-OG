// SPDX-License-Identifier: Apache-2.0
/*
 * driver/xilix/mpsoc/device/ps_uart.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_XILIX_MPSOC_DEVICE_PS_UART_H
#define DRIVER_XILIX_MPSOC_DEVICE_PS_UART_H

/* includes */

#include "lib/bit.h"

/* defines */

#define PS_UART_CONTROL                 0x0000
#define PS_UART_MODE                    0x0004
#define PS_UART_INTRPT_EN               0x0008
#define PS_UART_INTRPT_DIS              0x000c
#define PS_UART_INTRPT_MASK             0x0010
#define PS_UART_CHNL_INT_STS            0x0014
#define PS_UART_BAUD_RATE_GEN           0x0018
#define PS_UART_RCVR_TIMEOUT            0x001c
#define PS_UART_RCVR_FIFO_TRIGGER_LEVEL 0x0020
#define PS_UART_MODEM_CTRL              0x0024
#define PS_UART_MODEM_STS               0x0028
#define PS_UART_CHANNEL_STS             0x002c
#define PS_UART_TX_RX_FIFO              0x0030
#define PS_UART_BAUD_RATE_DIVIDER       0x0034
#define PS_UART_FLOW_DELAY              0x0038
#define PS_UART_TX_FIFO_TRIGGER_LEVEL   0x0044
#define UART_RX_FIFO_BYTE_STATUS        0x0048

/* control register */

#define CTRL_STPBRK                     BIT(8)
#define CTRL_STTBRK                     BIT(7)
#define CTRL_RSTTO                      BIT(6)
#define CTRL_TXDIS                      BIT(5)
#define CTRL_TXEN                       BIT(4)
#define CTRL_RXDIS                      BIT(3)
#define CTRL_RXEN                       BIT(2)
#define CTRL_TXRES                      BIT(1)
#define CTRL_RXRES                      BIT(0)

/* mode register */

#define MODE_WSIZE(d)                   BF_VALUE(d, 13, 12)
#define MODE_CHMODE(d)                  BF_VALUE(d, 9, 8)
#define MODE_NBSTOP(d)                  BF_VALUE(d, 7, 6)
#define MODE_PAR(d)                     BF_VALUE(d, 5, 3)
#define MODE_CHRL(d)                    BF_VALUE(d, 2, 1)
#define MODE_CLKS(d)                    BF_VALUE(d, 0, 0)

/* interrupt enable/disable/mask/status register */

#define INTR_RBRK                       BIT(13)
#define INTR_TOVR                       BIT(12)
#define INTR_TNFUL                      BIT(11)
#define INTR_TTRIG                      BIT(10)
#define INTR_DMSI                       BIT(9)
#define INTR_TIMEOUT                    BIT(8)
#define INTR_PARE                       BIT(7)
#define INTR_FRAME                      BIT(6)
#define INTR_ROVR                       BIT(5)
#define INTR_TFUL                       BIT(4)
#define INTR_TEMPTY                     BIT(3)
#define INTR_RFUL                       BIT(2)
#define INTR_REMPTY                     BIT(1)
#define INTR_RTRIG                      BIT(0)

/* modem control register */

#define MCTRL_FCM                       BIT(5)
#define MCTRL_RTS                       BIT(1)
#define MCTRL_DTR                       BIT(0)

/* modem status register */

#define MSTS_FCMS                       BIT(8)
#define MSTS_DCD                        BIT(7)
#define MSTS_RI                         BIT(6)
#define MSTS_DSR                        BIT(5)
#define MSTS_CTS                        BIT(4)
#define MSTS_DDCD                       BIT(3)
#define MSTS_TERI                       BIT(2)
#define MSTS_DDSR                       BIT(1)
#define MSTS_DCTS                       BIT(0)

/* channel status register */

#define CHSTS_TNFUL                     BIT(14)
#define CHSTS_TTRIG                     BIT(13)
#define CHSTS_FDELT                     BIT(12)
#define CHSTS_TACTIVE                   BIT(11)
#define CHSTS_RACTIVE                   BIT(10)
#define CHSTS_TFUL                      BIT(4)
#define CHSTS_TEMPTY                    BIT(3)
#define CHSTS_RFUL                      BIT(2)
#define CHSTS_REMPTY                    BIT(1)
#define CHSTS_RTRIG                     BIT(0)

/* Rx FIFO byte status register */

#define RFBS_BYTE3_BREAK                BIT(11)
#define RFBS_BYTE3_FRM_ERR              BIT(10)
#define RFBS_BYTE3_PAR_ERR              BIT(9)
#define RFBS_BYTE2_BREAK                BIT(8)
#define RFBS_BYTE2_FRM_ERR              BIT(7)
#define RFBS_BYTE2_PAR_ERR              BIT(6)
#define RFBS_BYTE1_BREAK                BIT(5)
#define RFBS_BYTE1_FRM_ERR              BIT(4)
#define RFBS_BYTE1_PAR_ERR              BIT(3)
#define RFBS_BYTE0_BREAK                BIT(2)
#define RFBS_BYTE0_FRM_ERR              BIT(1)
#define RFBS_BYTE0_PAR_ERR              BIT(0)

#endif /* DRIVER_XILIX_MPSOC_DEVICE_PS_UART_H */
