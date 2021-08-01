/*
 * src/log/log_local.h
 *
 * (C) 2018 Hidekazu Kato
 */

#ifndef SRC_LOG_LOG_LOCAL_H
#define SRC_LOG_LOG_LOCAL_H

/* includes */

/* defines */

#ifndef EOS
#define	EOS			'\0'
#endif

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "lib/system/errno.h"
#include "lib/log.h"

/* defines */

/* types */

/* variables */

/* functions */

static inline errno_t put_char(struct log_context *ctx, char ch)
{
	errno_t ret;

	ret = (*(ctx->request.ops->putc))(ctx, ch);

	return ret;
}

static inline char get_char(struct log_context *ctx)
{
	int ch;

	ch = *((ctx->request.format)++);

	return ch;
}

int log_output_unsigned_number(struct log_context *ctx);
int log_output_di(struct log_context *ctx);
int log_output_c(struct log_context *ctx);
int log_output_s(struct log_context *ctx);
int log_output_p(struct log_context *ctx);
int log_output_pads(struct log_context *ctx);
int log_output_prefix(struct log_context *ctx);
int log_output_string(struct log_context *ctx);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* SRC_LOG_LOG_LOCAL_H */

