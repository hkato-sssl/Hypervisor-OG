// SPDX-License-Identifier: Apache-2.0
/*
 * log/log_output_unsigned_number.c
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

static int output_llu(struct log_context *ctx, unsigned long long v)
{
    errno_t ret;
    int i;
    int ch;
    int radix;
    char *str;
    const char *char_table;

    if (ctx->syntax.flag.zero && (! ctx->syntax.flag.minus)) {
        ctx->output.pad = '0';
    } else {
        ctx->output.pad = ' ';
    }

    ctx->output.sign = EOS;

    i = 0;
    str = ctx->output.string.buffer;

    radix = ctx->syntax.radix->number;
    char_table = ctx->syntax.radix->characters;
    do {
        ch = (int)(v % radix);
        str[i++] = char_table[ch];
        v /= radix;
    } while (v != 0);
    ctx->output.string.length = i;

    ret = log_output_string(ctx);

    return ret;
}

int log_output_unsigned_number(struct log_context *ctx)
{
    errno_t ret;
    unsigned long long v;

    switch (ctx->syntax.length) {
    case CFL_HH:
        v = (unsigned char)va_arg(ctx->request.vargs, unsigned int);
        ret = output_llu(ctx, v);
        break;
    case CFL_H:
        v = (unsigned short)va_arg(ctx->request.vargs, unsigned int);
        ret = output_llu(ctx, v);
        break;
    case CFL_NONE:
        v = va_arg(ctx->request.vargs, unsigned int);
        ret = output_llu(ctx, v);
        break;
    case CFL_L:
        v = va_arg(ctx->request.vargs, unsigned long);
        ret = output_llu(ctx, v);
        break;
    case CFL_LL:
        v = va_arg(ctx->request.vargs, unsigned long long);
        ret = output_llu(ctx, v);
        break;
    case CFL_Z:
        v = (unsigned long long)va_arg(ctx->request.vargs, size_t);
        ret = output_llu(ctx, v);
        break;
    case CFL_T:
        v = va_arg(ctx->request.vargs, ptrdiff_t);
        ret = output_llu(ctx, v);
        break;
    default:
        ret = -EINVAL;
        break;
    }

    return ret;
}
