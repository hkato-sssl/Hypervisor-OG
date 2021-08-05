/*
 * system/stdc/stdio/sprintf.c
 *
 * (C) 2018 Hidekazu Kato
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

int sprintf(char *str, const char *fmt, ...)
{
    int ret;
    va_list vargs;

    va_start(vargs, fmt);
    ret = vsprintf(str, fmt, vargs);
    va_end(vargs);

    return ret;
}
