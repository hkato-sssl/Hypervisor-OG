/*
 * driver/xilinx/axi/uart_lite.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_XILINX_AXI_UART_LITE_H
#define DRIVER_XILINX_AXI_UART_LITE_H

/* includes */

#include <stdint.h>
#include <stdbool.h>
#include "lib/system/errno.h"

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* defines */

/* types */

struct uart_lite_configuration {
    uintptr_t   base;
    uint8_t     intr_no;
    struct {
        bool    init;
    } boolean;
};

struct uart_lite {
    uintptr_t   base;
    uint8_t     intr_no;
};

/* variables */

/* functions */

errno_t uart_lite_init(struct uart_lite *uart, const struct uart_lite_configuration *conf);
errno_t uart_lite_putc_poll(struct uart_lite *uart, int ch);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_XILINX_AXI_UART_LITE_H */

