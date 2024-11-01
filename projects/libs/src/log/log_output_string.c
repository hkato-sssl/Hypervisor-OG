// SPDX-License-Identifier: Apache-2.0
/*
 * log/log_output_string.c
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

static int output_string(struct log_context *ctx)
{
    errno_t ret;
    size_t idx;

    idx = ctx->output.string.length;
    if (idx > 0) {
        do {
            ret = put_char(ctx, ctx->output.string.buffer[--idx]);
        } while ((ret == SUCCESS) && (idx > 0));
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static int output_left_alignment(struct log_context *ctx)
{
    errno_t ret;

    ret = log_output_prefix(ctx);
    if (ret == SUCCESS) {
        ret = output_string(ctx);
        if (ret == SUCCESS) {
            ret = log_output_pads(ctx);
        }
    }

    return ret;
}

static int output_right_alignment(struct log_context *ctx)
{
    errno_t ret;

    if (ctx->syntax.flag.zero) {
        ret = log_output_prefix(ctx);
        if (ret == SUCCESS) {
            ret = log_output_pads(ctx);
            if (ret == SUCCESS) {
                ret = output_string(ctx);
            }
        }
    } else {
        ret = log_output_pads(ctx);
        if (ret == SUCCESS) {
            ret = log_output_prefix(ctx);
            if (ret == SUCCESS) {
                ret = output_string(ctx);
            }
        }
    }

    return ret;
}

int log_output_string(struct log_context *ctx)
{
    errno_t ret;

    if (ctx->syntax.flag.minus) {
        ret = output_left_alignment(ctx);
    } else {
        ret = output_right_alignment(ctx);
    }

    return ret;
}
