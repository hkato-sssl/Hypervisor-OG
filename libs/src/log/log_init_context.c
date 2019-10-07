/*
 * log/log_init_context.c
 *
 * (C) 2018 Hidekazu Kato
 */

#include <stddef.h>
#include <string.h>
#include "lib/system/errno.h"
#include "lib/log.h"

/* defines */

/* types */

/* variables */

/* functions */

int log_init_context(struct log_context *ctx, const struct log_context_configuration *conf)
{
	errno_t ret;

	if ((ctx != NULL) && (conf != NULL)) {
		memset(ctx, 0, sizeof(*ctx));
		ctx->arg = conf->arg;
		ctx->putc = conf->putc;
		ret = SUCCESS;
	} else {
		ret = -EINVAL;
	}

	return ret;
}

