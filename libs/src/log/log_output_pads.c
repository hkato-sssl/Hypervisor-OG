/*
 * log/log_output_pads.c
 *
 * (C) 2018 Hidekazu Kato
 */

#include <stddef.h>
#include "lib/log.h"
#include "log_local.h"

/* defines */

/* types */

/* variables */

/* functions */

static int string_length(struct log_context *ctx)
{
	int len;

	len = ctx->output.string.length;

	if (ctx->syntax.flag.hash && (ctx->syntax.radix != NULL)) {
		len += ctx->syntax.radix->prefix.length;
	}

	if (ctx->output.sign != EOS) {
		++len;
	}

	return len;
}

int log_output_pads(struct log_context *ctx)
{
	int ret;
	size_t len;

	len = string_length(ctx);

	if (ctx->syntax.width > len) {
		len = ctx->syntax.width - len;
		do {
			ret = put_char(ctx, ctx->output.pad);
		} while ((ret == SUCCESS) && (--len > 0));
	} else {
		ret = SUCCESS;
	}

	return ret;
}
