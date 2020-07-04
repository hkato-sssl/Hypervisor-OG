/*
 * lib/system/stdc/string.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef LIB_SYSTEM_STDC_STRING_H
#define LIB_SYSTEM_STDC_STRING_H

/* includes */

#include <stddef.h>

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stddef.h>

/* defines */

/* types */

/* variables */

/* functions */

void *memset(void *s, int c, size_t n);
void *memcpy(void *dst, const void *src, size_t n);
size_t strlen(const char *s);
char *strncpy(char *dst, const char *src, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_SYSTEM_STDC_STRING_H */

