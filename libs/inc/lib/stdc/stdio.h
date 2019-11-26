/*
 * lib/stdc/stdio.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef LIB_STDC_STDIO_H
#define LIB_STDC_STDIO_H

/* includes */

#include <stddef.h>

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* defines */

/* types */

/* variables */

/* functions */

int sprintf(char *str, const char *format, ...);
int snprintf(char *str, size_t size, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_STDC_STDIO_H */

