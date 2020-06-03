/*
 * thread/thread_local.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef THREAD_THREAD_LOCAL_H
#define THREAD_THREAD_LOCAL_H

/* includes */

/* defines */

#define THREAD_SETUP_ENTRY  0
#define THREAD_SETUP_ARG0   1

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>

/* defines */

/* types */

/* variables */

/* functions */

void *thread_stack_top(void);
uintptr_t *thread_setup_array(uint32_t no);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* THREAD_THREAD_LOCAL_H */

