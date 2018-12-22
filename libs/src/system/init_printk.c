/*
 * system/init_printk.c
 *
 * (C) 2018 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "lib/system/init.h"
#include "lib/log.h"

/* defines */

/* types */

/* variables */

static struct log_context ctx;

/* functions */

static int put_char(struct log_context *ctx, char ch)
{
	putchar(ch);

	return SUCCESS;
}

int init_printk(void)
{
	int ret;
	struct log_context_configuration conf;

	conf.putc = put_char;
	conf.arg = NULL;
	ret = log_init_context(&ctx, &conf);
	
	return ret;
}

int printk(const char *fmt, ...)
{
	int ret;

	ctx.fmt = fmt;
	va_start(ctx.vargs, fmt);
	ret = log_cformat(&ctx);
	va_end(ctx.vargs);

	return ret;
}

