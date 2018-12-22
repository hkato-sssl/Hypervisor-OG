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
	int i;
	char *str;
	char ch;

	if (ctx->syntax.flags.zero) {
		ctx->string.pad = '0';
	} else {
		ctx->string.pad = ' ';
	}

	if (v < 0) {
		ctx->string.sign = '-';
	} else if (ctx->syntax.flags.plus && (v > 0)) {
		ctx->string.sign = '+';
	} else {
		ctx->string.sign = EOS;
	}

	i = 0;
	str = ctx->string.buff;

	do {
		ch = v % 10;
		if (ch >= 0) {
			ch = '0' + ch;
		} else {
			ch = '0' - ch;
		}
		str[i++] = ch;
		v /= 10;
	} while (v != 0);
	ctx->string.len = i;

	ret = log_output_string(ctx);

	return SUCCESS;
}

int log_output_di(struct log_context *ctx)
{
	int ret;
	long long v;

	switch (ctx->syntax.length) {
	case CFL_HH:
		v = (char)va_arg(ctx->vargs, int);
		ret = output_lldi(ctx, v);
		break;
	case CFL_H:
		v = (short)va_arg(ctx->vargs, int);
		ret = output_lldi(ctx, v);
		break;
	case CFL_NONE:
		v = va_arg(ctx->vargs, int);
		ret = output_lldi(ctx, v);
		break;
	case CFL_L:
		v = va_arg(ctx->vargs, long);
		ret = output_lldi(ctx, v);
		break;
	case CFL_LL:
		v = va_arg(ctx->vargs, long long);
		ret = output_lldi(ctx, v);
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}
