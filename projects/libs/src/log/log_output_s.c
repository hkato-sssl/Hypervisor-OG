/*
 * log/log_output_s.c
 *
 * (C) 2018 Hidekazu Kato
 */

#include "lib/log.h"
#include "lib/system/errno.h"
#include "log_local.h"
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

/* defines */

/* types */

/* variables */

/* functions */

static int output_string(struct log_context *ctx, char *v)
{
    errno_t ret;
    char ch;

    ret = SUCCESS;

    while ((ch = *v++) != EOS) {
        ret = put_char(ctx, ch);
        if (ret != SUCCESS) {
            break;
        }
    }

    return ret;
}

static int output_left_alignment(struct log_context *ctx, char *v)
{
    errno_t ret;

    ret = output_string(ctx, v);
    if (ret == SUCCESS) {
        ret = log_output_pads(ctx);
    }

    return ret;
}

static int output_right_alignment(struct log_context *ctx, char *v)
{
    errno_t ret;

    ret = log_output_pads(ctx);
    if (ret == SUCCESS) {
        ret = output_string(ctx, v);
    }

    return ret;
}

int log_output_s(struct log_context *ctx)
{
    static char null_str[] = "<NULL>";

    errno_t ret;
    char *v;

    ctx->output.pad = ' ';

    v = va_arg(ctx->request.vargs, char *);
    if (v == NULL) {
        v = null_str;
    }
    ctx->output.string.length = strlen(v);

    if (ctx->syntax.flag.minus) {
        ret = output_left_alignment(ctx, v);
    } else {
        ret = output_right_alignment(ctx, v);
    }

    return ret;
}
