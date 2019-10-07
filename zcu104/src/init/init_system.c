/*
 * init/init_system.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "lib/system/errno.h"
#include "lib/log.h"
#include "driver/xilinx/axi/uart_lite.h"

/* defines */

#define	UART_LITE_BASE		0xa0001000

/* types */

/* prototypes */

/* variables */

static struct log_context log_ctx;
static struct uart_lite uart;

/* functions */

static errno_t put_char(struct log_context *ctx, char ch)
{
	errno_t ret;

    ret = uart_lite_poll_putc(ctx->arg, ch);

    return ret;
}

static errno_t init_uart(void)
{
	errno_t ret;
	struct uart_lite_configuration config;

	memset(&config, 0, sizeof(config));
    config.base = UART_LITE_BASE;
	config.flag.init = true;
	ret = uart_lite_init(&uart, &config);

	return ret;
}

static errno_t init_printk(void)
{
    errno_t ret;
	struct log_context_configuration config;

	ret = init_uart();
	if (ret == SUCCESS) {
		memset(&config, 0, sizeof(config));
		config.arg = &uart;
    	config.putc = put_char;
		ret = log_init_context(&log_ctx, &config);
	}

	return ret;
}

errno_t init_system(void)
{
	errno_t ret;

 	ret = init_printk();

	return ret;
}

