/*
 * xilinx/axi/uart_lite.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/xilinx/axi/uart_lite.h"
#include "driver/xilinx/axi/device/uart_lite.h"
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t init(struct uart_lite *uart,
                    const struct uart_lite_configuration *config)
{
    uint32_t d;

    uart->base = config->base;
    uart->intr_no = config->intr_no;

    /* initialize H/W */

    if (config->boolean.init) {
        d = CTRL_RST_TX_FIFO | CTRL_RST_RX_FIFO;
    } else {
        d = 0;
    }
    REG_WRITE32_SYNC(uart->base, UART_LITE_CTRL_REG, d);

    return SUCCESS;
}

errno_t uart_lite_init(struct uart_lite *uart,
                       const struct uart_lite_configuration *config)
{
    errno_t ret;

    if ((uart != NULL) && (config != NULL) && (config->base != 0)) {
        ret = init(uart, config);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t poll_putc(struct uart_lite *uart, int ch)
{
    uint32_t d;

    do {
        d = REG_READ32(uart->base, UART_LITE_STAT_REG);
    } while ((d & STAT_TX_FIFO_FULL) != 0);

    REG_WRITE32(uart->base, UART_LITE_TX_FIFO, ch);

    return SUCCESS;
}

errno_t uart_lite_putc_poll(struct uart_lite *uart, int ch)
{
    errno_t ret;

    if (uart != NULL) {
        ret = poll_putc(uart, ch);
    } else {
        ret = -EINVAL;
    }

    return ret;
}
