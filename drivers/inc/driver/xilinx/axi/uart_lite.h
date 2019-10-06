/*
 * driver/xilinx/axi/uart_lite.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_XILINX_AXI_UART_LITE_H
#define DRIVER_XILINX_AXI_UART_LITE_H

/* includes */

#include "lib/system/errno.h"
#include "driver/xilinx/axi/device/uart_lite.h"

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* defines */

/* types */

struct config_uart_lite {
	void		*base;
	uint8_t		intr_no;
};

struct uart_lite {
	void		*base;
	uint8_t		intr_no;
};

/* variables */

/* functions */

errno_t uart_lite_init(struct uart_lite *uart, const struct config_uart_lite *config);
errno_t uart_lite_putc(struct uart_lite *uart, int ch);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_XILINX_AXI_UART_LITE_H */

