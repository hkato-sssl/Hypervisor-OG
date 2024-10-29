// SPDX-License-Identifier: Apache-2.0
/*
 * log/log_output_prefix.c
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

static int output_radix(struct log_context *ctx)
{
    errno_t ret;
    const char *str;
    size_t idx;
    size_t len;

    if (ctx->syntax.flag.hash && (ctx->syntax.radix != NULL)
        && (ctx->syntax.radix->prefix.length > 0)) {
        len = ctx->syntax.radix->prefix.length;
        str = ctx->syntax.radix->prefix.string;
        for (idx = 0; idx < len; ++idx) {
            ret = put_char(ctx, str[idx]);
            if (ret != SUCCESS) {
                break;
            }
        }
    } else {
        /* no work */
        ret = SUCCESS;
    }

    return ret;
}

static int output_sign(struct log_context *ctx)
{
    errno_t ret;

    if (ctx->output.sign != EOS) {
        ret = put_char(ctx, ctx->output.sign);
        if (ret == SUCCESS) {
            ret = output_radix(ctx);
        }
    } else {
        ret = output_radix(ctx);
    }

    return ret;
}

int log_output_prefix(struct log_context *ctx)
{
    errno_t ret;

    ret = output_sign(ctx);

    return ret;
}
