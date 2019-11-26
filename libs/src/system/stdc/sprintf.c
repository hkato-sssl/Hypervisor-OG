/*
 * lib/stdc/sprintf.c
 *
 * (C) 2018 Hidekazu Kato
 */

#include <stddef.h>
#include <stdarg.h>
#include "lib/system/stdc/stdio.h"

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

