/*
 * log/log_output_di.c
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

static int output_lldi(struct log_context *ctx, long long v)
{
	int ret;
	char *str;
	char ch;
	size_t idx;

	if (ctx->syntax.flag.zero) {
		ctx->output.pad = '0';
	} else {
		ctx->output.pad = ' ';
	}

	if (v < 0) {
		ctx->output.sign = '-';
	} else if (ctx->syntax.flag.plus) {
		ctx->output.sign = '+';
	} else if (ctx->syntax.flag.space) {
		ctx->output.sign = ' ';
	} else {
		ctx->output.sign = EOS;
	}

	idx = 0;
	str = ctx->output.string.buffer;

	do {
		ch = v % 10;
		if (ch >= 0) {
			ch = '0' + ch;
		} else {
			ch = '0' - ch;
		}
		str[idx++] = ch;
		v /= 10;
	} while (v != 0);
	ctx->output.string.length = idx;

	ret = log_output_string(ctx);

	return ret;
}

int log_output_di(struct log_context *ctx)
{
	int ret;
	long long v;

	switch (ctx->syntax.length) {
	case CFL_HH:
		v = (char)va_arg(ctx->input.vargs, int);
		ret = output_lldi(ctx, v);
		break;
	case CFL_H:
		v = (short)va_arg(ctx->input.vargs, int);
		ret = output_lldi(ctx, v);
		break;
	case CFL_NONE:
		v = va_arg(ctx->input.vargs, int);
		ret = output_lldi(ctx, v);
		break;
	case CFL_L:
		v = va_arg(ctx->input.vargs, long);
		ret = output_lldi(ctx, v);
		break;
	case CFL_LL:
		v = va_arg(ctx->input.vargs, long long);
		ret = output_lldi(ctx, v);
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}
