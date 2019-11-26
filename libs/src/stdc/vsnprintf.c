/*
 * lib/stdc/vsnprintf.c
 *
 * (C) 2018 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "lib/system/spin_lock.h"
#include "lib/log.h"
#include "lib/stdc/stdio.h"

/* defines */

/* types */

struct arg_snprintf {
    char    *buff;
    size_t  buff_sz;
    size_t  ct;
    bool    over_flow;
};

/* prototypes */

/* variables */

static struct log_context ctx;
static struct log_ops ops; 

/* functions */

static int put_char(struct log_context *ctx, char ch)
{
    int ret;
    char *p;
    struct arg_snprintf *arg;

    arg = ctx->request.ops->arg;
    if (arg->ct < arg->buff_sz) {
        p = arg->buff;
        *p++ = ch;
        arg->buff = p;
        ++(arg->ct);
        ret = SUCCESS;
    } else {
        arg->over_flow = true;
        ret = -ENOMEM;
    }

    return ret;
}

static int exec_vsnprintf(char *str, size_t size, const char *fmt, va_list ap)
{
    int ret;
    struct arg_snprintf putc_arg;

    putc_arg.buff = str;
    putc_arg.buff_sz = size;
    putc_arg.ct = 0;
    putc_arg.over_flow = false;
    
    system_spin_lock();
    
    ops.putc = put_char;
    ops.arg = &putc_arg;
    ctx.request.ops = &ops;
    ctx.request.format = fmt;
    va_copy(ctx.request.vargs, ap);
    ret = log_cformat(&ctx);
    va_end(ctx.request.vargs);
    
    system_spin_unlock();

    if (putc_arg.over_flow || (putc_arg.ct == size)) {
        ret = (int)size;
    } else if (ret == SUCCESS) {
        *(putc_arg.buff) = '\0';
        ret = (int)(putc_arg.ct);
    }

    return ret;
}

int vsnprintf(char *str, size_t size, const char *fmt, va_list ap)
{
    int ret;

    if (size <= INT_MAX) {
        ret = exec_vsnprintf(str, size, fmt, ap);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

