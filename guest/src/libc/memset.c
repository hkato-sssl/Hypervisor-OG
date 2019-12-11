#include <stddef.h>
#include <string.h>

void *memset(void *s, int c, size_t n)
{
    volatile char *p;

    p = s;
    for (; n > 0; --n) {
        *p++ = (char)c;
    }

    return s;
}
