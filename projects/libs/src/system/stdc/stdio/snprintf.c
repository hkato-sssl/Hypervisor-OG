/*
 * system/stdc/stdio/snprintf.c
 *
 * (C) 2018 Hidekazu Kato
 */

#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

int snprintf(char *str, size_t size, const char *fmt, ...)
{
    int ret;
    va_list vargs;

    va_start(vargs, fmt);
    ret = vsnprintf(str, size, fmt, vargs);
    va_end(vargs);

    return ret;
}

