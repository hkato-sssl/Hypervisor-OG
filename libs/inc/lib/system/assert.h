/*
 * system/assert.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef SYSTEM_ASSERT_H
#define SYSTEM_ASSERT_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* defines */

#define SYSTEM_ASSERT(exp) do { if (! (exp)) system_assert(__func__, __LINE__); } while (0)

/* types */

/* variables */

/* inline functions */

static inline void system_assert(char const *func, uint32_t line)
{
    __asm volatile ("svc #0xffff");
}

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* SYSTEM_ASSERT_H */

