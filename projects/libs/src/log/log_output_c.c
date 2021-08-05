/*
 * log/log_output_c.c
 *
 * (C) 2018 Hidekazu Kato
 */

#include "lib/log.h"
#include "lib/system/errno.h"
#include "log_local.h"
#include <stdarg.h>
#include <stddef.h>

/* defines */

/* types */

/* variables */

/* functions */

static int output_c(struct log_context *ctx, char v)
{
    errno_t ret;

    ctx->output.pad = ' ';
    ctx->output.sign = EOS;
    ctx->output.string.length = 1;
    ctx->output.string.buffer[0] = v;
    ret = log_output_string(ctx);

    return ret;
}

int log_output_c(struct log_context *ctx)
{
    errno_t ret;
    char v;

    switch (ctx->syntax.length) {
    case CFL_HH:
        v = (char)va_arg(ctx->request.vargs, int);
        ret = output_c(ctx, v);
        break;
    case CFL_H:
        v = (char)va_arg(ctx->request.vargs, int);
        ret = output_c(ctx, v);
        break;
    case CFL_NONE:
        v = (char)va_arg(ctx->request.vargs, int);
        ret = output_c(ctx, v);
        break;
    case CFL_L:
        v = (char)va_arg(ctx->request.vargs, long);
        ret = output_c(ctx, v);
        break;
    case CFL_LL:
        v = (char)va_arg(ctx->request.vargs, long long);
        ret = output_c(ctx, v);
        break;
    default:
        ret = -EINVAL;
        break;
    }

    return ret;
}
