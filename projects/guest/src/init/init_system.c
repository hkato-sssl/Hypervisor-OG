// SPDX-License-Identifier: Apache-2.0
/*
 * init/init_system.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/aarch64.h"
#include "driver/aarch64/system_register.h"
#include "driver/system/cpu.h"
#include "driver/xilinx/mpsoc/device/ps_uart.h"
#include "driver/xilinx/mpsoc/ps_uart.h"
#include "lib/log.h"
#include "lib/system.h"
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "lib/system/printk.h"
#include "lib/system/spin_lock.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* defines */

#define UART0_BASE 0xff000000

/* types */

struct arg_printk {
    uintptr_t reg_base;
    int ct;
};

/* prototypes */

static errno_t put_char(struct log_context *ctx, char ch);

/* variables */

static struct log_context log_ctx;
static struct log_ops ops = {NULL, put_char};
static spin_lock_t lock;
static spin_lock_t system_lock;

/* functions */

errno_t printk(const char *fmt, ...)
{
    errno_t ret;
    va_list vargs;
    struct arg_printk arg;

    arg.reg_base = UART0_BASE;
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
        ret = ps_uart_putc_poll(arg->reg_base, '\r');
        if (ret == SUCCESS) {
            ret = ps_uart_putc_poll(arg->reg_base, ch);
        }
        arg->ct += 2;
    } else {
        ret = ps_uart_putc_poll(arg->reg_base, ch);
        ++(arg->ct);
    }

    return ret;
}

static errno_t init_uart(void)
{
    REG_WRITE32_SYNC(UART0_BASE, PS_UART_INTRPT_MASK, 0xff);

    return SUCCESS;
}

static errno_t init_printk(void)
{
    errno_t ret;

    spin_lock_init(&lock);
    ret = init_uart();

    return ret;
}

static errno_t init_exception(void)
{
    extern char excvec_el1[];
    void *exc_stack_top(void);

    void *sp;

    sp = exc_stack_top();
    aarch64_write_tpidr_el1((uintptr_t)sp);

    aarch64_write_vbar_el1((uint64_t)excvec_el1);
    aarch64_isb();

    return SUCCESS;
}

errno_t init_system(void)
{
    errno_t ret;

    if (cpu_no() == 0) {
        ret = init_printk();
        if (ret == SUCCESS) {
            ret = init_exception();
        }
        if (ret == SUCCESS) {
            ret = system_register_spin_lock(&system_lock);
        }
    } else {
        ret = init_exception();
    }

    return ret;
}
