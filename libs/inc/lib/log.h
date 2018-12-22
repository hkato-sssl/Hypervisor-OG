/*
 * lib/log.h
 *
 * (C) 2018 Hidekazu Kato
 */

#ifndef LIB_LOG_H
#define LIB_LOG_H

/* includes */

/* defines */

#ifndef ASSEMBLER

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
#include "lib/config.h"
#include "lib/errno.h"

/* defines */

#define	MAX_LOG_STR_LEN	24

/* types */

struct log_context;

typedef	int (*log_putc_func_t)(struct log_context *ctx, const char ch);

enum log_cfmt_length {
	CFL_NONE,		/* no flag */
	CFL_HH,			/* char */
	CFL_H,			/* short */
	CFL_L,			/* long */
	CFL_LL,			/* long long */
	CFL_Z,			/* size_t */
	CFL_T,			/* ptrdiff_t */
};

struct log_cformat_syntax {
	struct {
		bool				minus;
		bool				plus;
		bool				space;
		bool				hash;
		bool				zero;
	} flags;
	char					precision;
	char					type;
	size_t 					width;
	enum log_cfmt_length	length;
};

struct log_context {
	void						*arg;
	log_putc_func_t				putc;
    const char 	    		 	*fmt;
    va_list    	    		 	vargs;
	struct log_cformat_syntax	syntax;
	struct {
		size_t					len;
		char					sign;
		char					pad;
		char					buff[MAX_LOG_STR_LEN];
	} string;
};

struct log_context_configuration {
	void				*arg;
	log_putc_func_t		putc;
};

/* variables */

/* functions */

int log_init_context(struct log_context *ctx, const struct log_context_configuration *conf);
int log_cformat(struct log_context *ctx);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLER */

#endif /* LIB_LOG_H */

