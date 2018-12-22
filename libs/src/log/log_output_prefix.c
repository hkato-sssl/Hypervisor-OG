/*
 * log/log_output_prefix.c
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

static int output_radix(struct log_context *ctx)
{
	int ret;
	size_t idx;

	if (ctx->syntax.flag.hash && (ctx->radix.prefix.length > 0)) {
		for (idx = 0; idx < ctx->radix.prefix.length; ++idx) {
			ret = put_char(ctx, ctx->radix.prefix.string[idx]);
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
	int ret;

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
	int ret;

	ret = output_sign(ctx);

	return ret;
}

