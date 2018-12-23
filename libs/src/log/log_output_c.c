/*
 * log/log_output_c.c
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

static int output_left_alignment(struct log_context *ctx, char v)
{
	int ret;

	ret = put_char(ctx, v);
	if (ret == SUCCESS) {
		ret = log_output_pads(ctx);
	}

	return ret;
}

static int output_right_alignment(struct log_context *ctx, char v)
{
	int ret;

	ret = log_output_pads(ctx);
	if (ret == SUCCESS) {
		ret = put_char(ctx, v);
	}

	return ret;
}

static int output_c(struct log_context *ctx, char v)
{
	int ret;

	ctx->output.pad = ' ';
	ctx->output.sign = EOS;

	if (ctx->syntax.flag.minus) {
		ret = output_left_alignment(ctx, v);
	} else {
		ret = output_right_alignment(ctx, v);
	}

	return ret;
}

int log_output_c(struct log_context *ctx)
{
	int ret;
	char v;

	switch (ctx->syntax.length) {
	case CFL_HH:
		v = (char)va_arg(ctx->input.vargs, int);
		ret = output_c(ctx, v);
		break;
	case CFL_H:
		v = (char)va_arg(ctx->input.vargs, int);
		ret = output_c(ctx, v);
		break;
	case CFL_NONE:
		v = (char)va_arg(ctx->input.vargs, int);
		ret = output_c(ctx, v);
		break;
	case CFL_L:
		v = (char)va_arg(ctx->input.vargs, long);
		ret = output_c(ctx, v);
		break;
	case CFL_LL:
		v = (char)va_arg(ctx->input.vargs, long long);
		ret = output_c(ctx, v);
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}
