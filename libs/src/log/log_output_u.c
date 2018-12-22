/*
 * log/log_output_u.c
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

static int output_llu(struct log_context *ctx, unsigned long long v)
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

	ctx->string.sign = EOS;

	i = 0;
	str = ctx->string.buff;

	do {
		ch = '0' + (char)(v % 10);
		str[i++] = ch;
		v /= 10;
	} while (v != 0);
	ctx->string.len = i;

	ret = log_output_string(ctx);

	return SUCCESS;
}

int log_output_u(struct log_context *ctx)
{
	int ret;
	unsigned long long v;

	switch (ctx->syntax.length) {
	case CFL_HH:
		v = (unsigned char)va_arg(ctx->vargs, unsigned int);
		ret = output_llu(ctx, v);
		break;
	case CFL_H:
		v = (unsigned short)va_arg(ctx->vargs, unsigned int);
		ret = output_llu(ctx, v);
		break;
	case CFL_NONE:
		v = va_arg(ctx->vargs, unsigned int);
		ret = output_llu(ctx, v);
		break;
	case CFL_L:
		v = va_arg(ctx->vargs, unsigned long);
		ret = output_llu(ctx, v);
		break;
	case CFL_LL:
		v = va_arg(ctx->vargs, unsigned long long);
		ret = output_llu(ctx, v);
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}
