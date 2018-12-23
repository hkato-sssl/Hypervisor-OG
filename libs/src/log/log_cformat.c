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

static char dec_chars[] = "0123456789";
static char oct_prefix[] = "0";
static char lower_case_hex_chars[] = "0123456789abcdef";
static char lower_case_hex_prefix[] = "0x";
static char upper_case_hex_chars[] = "0123456789ABCDEF";
static char upper_case_hex_prefix[] = "0X";

static struct log_radix dec = { 10, dec_chars, { 0, NULL } };
static struct log_radix oct = { 8, dec_chars, { 1, oct_prefix } };
static struct log_radix lower_case_hex = { 16, lower_case_hex_chars, { 2, lower_case_hex_prefix } };
static struct log_radix upper_case_hex = { 16, upper_case_hex_chars, { 2, upper_case_hex_prefix } };

/* functions */

static int parse_type(struct log_context *ctx, char ch)
{
	int ret;

	ctx->syntax.type = ch;

	switch (ch) {
	case 'd':
	case 'i':
		ctx->syntax.flag.hash = false;
		ret = log_output_di(ctx);
		break;
	case 'u':
		ctx->syntax.flag.hash = false;
		ctx->radix = dec;
		ret = log_output_unsigned_number(ctx);
		break;
	case 'o':
		ctx->radix = oct;
		ret = log_output_unsigned_number(ctx);
		break;
	case 'x':
		ctx->radix = lower_case_hex;
		ret = log_output_unsigned_number(ctx);
		break;
	case 'X':
		ctx->radix = upper_case_hex;
		ret = log_output_unsigned_number(ctx);
		break;
	case 'p':
		ctx->radix = lower_case_hex;
		ctx->syntax.flag.hash = true;
		ret = log_output_unsigned_number(ctx);
		break;
	case 'c':
		ctx->syntax.flag.hash = false;
		ret = log_output_c(ctx);
		break;
	case 's':
		ctx->syntax.flag.hash = false;
		ret = log_output_s(ctx);
		break;
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
		ctx->syntax.flag.minus = true;
		ret = parse_flag(ctx);
		break;
	case '+':
		ctx->syntax.flag.plus = true;
		ret = parse_flag(ctx);
		break;
	case ' ':
		ctx->syntax.flag.space = true;
		ret = parse_flag(ctx);
		break;
	case '#':
		ctx->syntax.flag.hash = true;
		ret = parse_flag(ctx);
		break;
	case '0':
		ctx->syntax.flag.zero = true;
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

static int output_formatted_string(struct log_context *ctx)
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
			ret = output_formatted_string(ctx);
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

	if ((ctx != NULL) && (ctx->putc != NULL) && (ctx->input.format != NULL)) {
		ret = cformat(ctx);
	} else {
		ret = -EINVAL;
	}

	return ret;
}

