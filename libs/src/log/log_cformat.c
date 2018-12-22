/*
 * log/log_cformat.c
 *
 * (C) 2018 Hidekazu Kato
 */

#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "lib/log.h"
#include "log_local.h"

/* types */

/* variables */

/* functions */

static int parse_type(struct log_context *ctx, char ch)
{
	int ret;

	ctx->syntax.type = ch;

	switch (ch) {
	case 'd':
	case 'i':
		ret = log_output_di(ctx);
		break;
	case 'u':
		ret = log_output_u(ctx);
		break;
#if 0
	case 'o':
		ret = log_output_o(ctx);
		break;
	case 'x':
		ret = log_output_x(ctx);
		break;
	case 'X':
		ret = log_output_upper_x(ctx);
		break;
	case 'c':
		ret = log_output_c(ctx);
		break;
	case 's':
		ret = log_output_s(ctx);
		break;
	case 'p':
		ret = log_output_p(ctx);
		break;
#endif
	case '%':
		ret = put_char(ctx, ch);
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

static int parse_length_h_hh(struct log_context *ctx)
{
	int ret;
	char ch;

	ch = get_char(ctx);
	if (ch == 'h') {
		ctx->syntax.length = CFL_HH;
		ch = get_char(ctx);
	} else {
		ctx->syntax.length = CFL_H;
	}

	ret = parse_type(ctx, ch);

	return ret;
}

static int parse_length_l_ll(struct log_context *ctx)
{
	int ret;
	char ch;

	ch = get_char(ctx);
	if (ch == 'l') {
		ctx->syntax.length = CFL_LL;
		ch = get_char(ctx);
	} else {
		ctx->syntax.length = CFL_L;
	}

	ret = parse_type(ctx, ch);

	return ret;
}
	
static int parse_length_others(struct log_context *ctx, char ch)
{
	int ret;

	switch (ch) {
	case 'z':
		ctx->syntax.length = CFL_Z;
		ch = get_char(ctx);
		break;
	case 't':
		ctx->syntax.length = CFL_T;
		ch = get_char(ctx);
		break;
	default:
		/* no work */
		break;
	}

	ret = parse_type(ctx, ch);

	return ret;
}

static int parse_length(struct log_context *ctx, char ch)
{
	int ret;

	switch (ch) {
	case 'h':
		ret = parse_length_h_hh(ctx);
		break;
	case 'l':
		ret = parse_length_l_ll(ctx);
		break;
	default:
		ret = parse_length_others(ctx, ch);
		break;
	}

	return ret;
}

static int parse_width(struct log_context *ctx, char ch)
{
	int ret;

	while ((ch >= '0') && (ch <= '9')) {
		ctx->syntax.width = ctx->syntax.width * 10 + (ch - '0');
		ch = get_char(ctx);
	}

	ret = parse_length(ctx, ch);

	return ret;
}

static int parse_flag(struct log_context *ctx)
{
	int ret;
	char ch;

	ch = get_char(ctx);

	switch (ch) {
	case EOS:
		ret = -EINVAL;
		break;
	case '-':
		ctx->syntax.flags.minus = true;
		ret = parse_flag(ctx);
		break;
	case '+':
		ctx->syntax.flags.plus = true;
		ret = parse_flag(ctx);
		break;
	case ' ':
		ctx->syntax.flags.space = true;
		ret = parse_flag(ctx);
		break;
	case '#':
		ctx->syntax.flags.hash = true;
		ret = parse_flag(ctx);
		break;
	case '0':
		ctx->syntax.flags.zero = true;
		ret = parse_flag(ctx);
		break;
	default:
		ret = parse_width(ctx, ch);
		break;
	}

	return ret;
}

static int parse_format(struct log_context *ctx)
{
	int ret;

	memset(&(ctx->syntax), 0, sizeof(ctx->syntax));
	ret = parse_flag(ctx);

	return ret;
}

static int out_formatted_string(struct log_context *ctx)
{
	int ret;

	ret = parse_format(ctx);

	return ret;
}

static int cformat(struct log_context *ctx)
{
	int ret;
	char ch;

	ret = SUCCESS;
	for (ch = get_char(ctx); ch != EOS; ch = get_char(ctx)) {
		if (ch == '%') {
			ret = out_formatted_string(ctx);
		} else {
			ret = put_char(ctx, ch);
		}

		if (ret != SUCCESS) {
			break;
		}
	}

	return ret;
}

int log_cformat(struct log_context *ctx)
{
	int ret;

	if ((ctx != NULL) && (ctx->putc != NULL) && (ctx->fmt != NULL)) {
		ret = cformat(ctx);
	} else {
		ret = -EINVAL;
	}

	return ret;
}

