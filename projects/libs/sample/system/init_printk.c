/*
 * system/init_printk.c
 *
 * (C) 2018 Hidekazu Kato
 */

#include "lib/log.h"
#include "lib/system/init.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* defines */

/* types */

/* variables */

static struct log_context ctx;

/* functions */

static int put_char(struct log_context *ctx, char ch)
{
    putchar(ch);

    return SUCCESS;
}

int init_printk(void)
{
    int ret;
    struct log_context_configuration conf;

    conf.putc = put_char;
    conf.arg = NULL;
    ret = log_init_context(&ctx, &conf);

    return ret;
}

int printk(const char *fmt, ...)
{
    int ret;

    ctx.input.format = fmt;
    va_start(ctx.input.vargs, fmt);
    ret = log_cformat(&ctx);
    va_end(ctx.input.vargs);

    return ret;
}
