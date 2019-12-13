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

void system_assert(const char *func, uint32_t line);
void system_alert(const char *func, uint32_t line, const char *message);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* SYSTEM_ASSERT_H */

