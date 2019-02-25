/*
 * lib/log.h
 *
 * (C) 2018 Hidekazu Kato
 */

#ifndef LIB_LOG_H
#define LIB_LOG_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
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

struct log_radix {
	int						number;
	const char				*characters;
	struct {
		int					length;
		const char			*string;
	} prefix;
};

struct log_cformat_syntax {
	struct {
		bool				minus;
		bool				plus;
		bool				space;
		bool				hash;
		bool				zero;
	} flag;
	char					precision;
	char					type;
	size_t 					width;
	enum log_cfmt_length	length;
	const struct log_radix	*radix;
};

struct log_context {
	void						*arg;
	log_putc_func_t				putc;

	struct {
    	const char     		 	*format;
    	va_list        		 	vargs;
	} input;

	struct log_cformat_syntax	syntax;

	struct {
		char					sign;
		char					pad;
		struct {
			size_t				length;
			char				buffer[MAX_LOG_STR_LEN];
		} string;
	} output;
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

#endif /* ASSEMBLY */

#endif /* LIB_LOG_H */

