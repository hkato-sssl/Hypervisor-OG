/*
 * lib/system/stdc/stdio.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef LIB_SYSTEM_STDC_STDIO_H
#define LIB_SYSTEM_STDC_STDIO_H

/* includes */

#include <stddef.h>
#include <stdarg.h>

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
int vsprintf(char *str, const char *format, va_list ap);
int vsnprintf(char *str, size_t size, const char *format, va_list ap);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_SYSTEM_STDC_STDIO_H */

