/*
 * system/init_printk.c
 *
 * (C) 2018 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "lib/system/spin_lock.h"
#include "lib/system.h"
#include "lib/log.h"

/* defines */

/* types */

struct arg_sprintf {
    char    *buff;
    int     ct;
};

/* prototypes */

/* variables */

static struct log_context ctx;
static struct log_ops ops; 

/* functions */

static int put_char(struct log_context *ctx, char ch)
{
    char *p;
    struct arg_sprintf *arg;

    arg = ctx->request.ops->arg;
    p = arg->buff;
    *p++ = ch;
    arg->buff = p;
    ++(arg->ct);

    return SUCCESS;
}

int sprintf(char *buff, const char *fmt, ...)
{
    int ret;
    struct arg_sprintf putc_arg;

    putc_arg.buff = buff;
    putc_arg.ct = 0;

    system_spin_lock();

    ops.putc = put_char;
    ops.arg = &putc_arg;
    ctx.request.ops = &ops;
    ctx.request.format = fmt;
    va_start(ctx.request.vargs, fmt);
    ret = log_cformat(&ctx);
    va_end(ctx.request.vargs);

    system_spin_unlock();

    if (ret == SUCCESS) {
        *(putc_arg.buff) = '\0';
        ret = putc_arg.ct;
    }

    return ret;
}
