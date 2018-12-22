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

int log_output_pads(struct log_context *ctx)
{
	int ret;
	size_t len;

	len = ctx->string.len;
	if (ctx->string.sign != EOS) {
		++len;
	}

	if (ctx->syntax.width > len) {
		len = ctx->syntax.width - len;
		do {
			ret = put_char(ctx, ctx->string.pad);
		} while ((ret == SUCCESS) && (--len > 0));
	} else {
		ret = SUCCESS;
	}

	return ret;
}

