/*
 * log/log_output_unsigned_number.c
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
	int ch;
	int radix;
	char *str;
	const char *char_table;

	if (ctx->syntax.flag.zero) {
		ctx->output.pad = '0';
	} else {
		ctx->output.pad = ' ';
	}

	ctx->output.sign = EOS;

	i = 0;
	str = ctx->output.string.buffer;

	radix = ctx->radix.number;
	char_table = ctx->radix.characters;
	do {
		ch = (int)(v % radix);
		str[i++] = char_table[ch];
		v /= radix;
	} while (v != 0);
	ctx->output.string.length = i;

	ret = log_output_string(ctx);

	return SUCCESS;
}

int log_output_unsigned_number(struct log_context *ctx)
{
	int ret;
	unsigned long long v;

	switch (ctx->syntax.length) {
	case CFL_HH:
		v = (unsigned char)va_arg(ctx->input.vargs, unsigned int);
		ret = output_llu(ctx, v);
		break;
	case CFL_H:
		v = (unsigned short)va_arg(ctx->input.vargs, unsigned int);
		ret = output_llu(ctx, v);
		break;
	case CFL_NONE:
		v = va_arg(ctx->input.vargs, unsigned int);
		ret = output_llu(ctx, v);
		break;
	case CFL_L:
		v = va_arg(ctx->input.vargs, unsigned long);
		ret = output_llu(ctx, v);
		break;
	case CFL_LL:
		v = va_arg(ctx->input.vargs, unsigned long long);
		ret = output_llu(ctx, v);
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}
