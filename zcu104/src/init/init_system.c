/*
 * init/init_system.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "lib/system/spin_lock.h"
#include "lib/system.h"
#include "lib/log.h"
#include "driver/aarch64.h"
#include "driver/aarch64/system_register.h"
#include "driver/xilinx/axi/uart_lite.h"
#include "hypervisor/tls.h"

/* defines */

#define UART_LITE_BASE      0xa0001000

/* types */

struct arg_printk {
    struct uart_lite    *uart;
    int                 ct;
};

/* prototypes */

static errno_t put_char(struct log_context *ctx, char ch);

/* variables */

static struct log_context log_ctx;
static struct uart_lite uart;
static struct log_ops ops = { &uart, put_char };
static spin_lock_t lock;
static spin_lock_t system_lock;

/* functions */

errno_t printk(const char *fmt, ...)
{
    errno_t ret;
    va_list vargs;
    struct arg_printk arg;

    arg.uart = &uart;
    arg.ct = 0;

    spin_lock(&lock);
    va_start(vargs, fmt);

    ops.arg = &arg;
    log_ctx.request.ops = &ops;
    log_ctx.request.format = fmt;
    log_ctx.request.vargs = vargs;
    ret = log_cformat(&log_ctx);

    va_end(vargs);
    spin_unlock(&lock);

    if (ret == SUCCESS) {
        ret = arg.ct;
    }

    return ret;
}

static errno_t put_char(struct log_context *ctx, char ch)
{
    errno_t ret;
    struct arg_printk *arg;

    arg = ctx->request.ops->arg;
    if (ch == '\n') {
        ret = uart_lite_poll_putc(arg->uart, '\r');
        if (ret == SUCCESS) {
            ret = uart_lite_poll_putc(arg->uart, ch);
        }
        arg->ct += 2;
    } else {
        ret = uart_lite_poll_putc(arg->uart, ch);
        ++(arg->ct);
    }

    return ret;
}

static errno_t init_uart(void)
{
    errno_t ret;
    struct uart_lite_configuration config;

    memset(&config, 0, sizeof(config));
    config.base = UART_LITE_BASE;
    config.boolean.init = true;
    ret = uart_lite_init(&uart, &config);

    return ret;
}

static errno_t init_printk(void)
{
    errno_t ret;

    spin_lock_init(&lock);
    ret = init_uart();

    return ret;
}

static errno_t init_tls(void)
{
    char *p;
    uintptr_t *tls;

    void *exc_stack_top(void);

    p = exc_stack_top();
    p -= TLS_SIZE;
    memset(p, 0, TLS_SIZE);

    tls = (uintptr_t *)p;
    tls[TLS_EXCEPTION_SP] = (uintptr_t)p;

    return SUCCESS;
}

static errno_t init_exception(void)
{
    extern char excvec_hyp[];
    void *exc_stack_top(void);
    void *p;

    p = exc_stack_top();
    aarch64_write_tpidr_el2((uintptr_t)p - TLS_SIZE);
    aarch64_isb();
    aarch64_write_vbar_el2((uint64_t)excvec_hyp);
    aarch64_isb();

    return SUCCESS;
}

errno_t init_system(void)
{
    errno_t ret;

    ret = init_printk();
    if (ret == SUCCESS) {
    	ret = init_exception();
    }
    if (ret == SUCCESS) {
        ret = init_system_spin_lock(&system_lock);
    }
    if (ret == SUCCESS) {
        ret = init_tls();
    }

    return ret;
}

