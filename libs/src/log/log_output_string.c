/*
 * log/log_output_string.c
 *
 * (C) 2018 Hidekazu Kato
 */

#include <stddef.h>
#include <stdarg.h>
#include "lib/log.h"
#include "log_local.h"

/* defines */

/* types */

/* variables */

/* functions */

static int output_postfix(struct log_context *ctx)
{
	int ret;

	if (ctx->syntax.flags.minus) {
		ret = log_output_pads(ctx);
	} else {
		ret = SUCCESS;
	}
			
	return ret;
}

static int output_buff(struct log_context *ctx)
{
	int ret;
	size_t i;

	i = ctx->string.len;
	if (i > 0) {
		do {
			ret = put_char(ctx, ctx->string.buff[--i]);
		} while ((ret == SUCCESS) && (i > 0));
	} else {
		ret = -EINVAL;
	}

	return ret;
}

static int output_sign(struct log_context *ctx)
{
	int ret;
	char sign;

	sign = ctx->string.sign;
	if (sign != EOS) {
		ret = put_char(ctx, sign);
	} else {
		ret = SUCCESS;
	}
}

static int output_prefix(struct log_context *ctx)
{
	int ret;

	if (! ctx->syntax.flags.minus) {
		ret = log_output_pads(ctx);
	} else {
		ret = SUCCESS;
	}

	return ret;
}

int log_output_string(struct log_context *ctx)
{
	int ret;

	ret = output_prefix(ctx);
	if (ret == SUCCESS) {
		ret = output_sign(ctx);
		if (ret == SUCCESS) {
			ret = output_buff(ctx);
			if (ret == SUCCESS) {
				ret = output_postfix(ctx);
			}
		}
	}

	return ret;
}

