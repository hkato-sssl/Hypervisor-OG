/*
 * thread/thread_local.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef THREAD_THREAD_LOCAL_H
#define THREAD_THREAD_LOCAL_H

/* includes */

#include "hypervisor/thread.h"

/* defines */

#define THREAD_PARAMETER_ENTRY      0
#define THREAD_PARAMETER_ARG(n)     ((n) + 1)
#define NR_THREAD_PARAMETERS        (1 + NR_THREAD_ARGS)

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
uintptr_t *thread_parameter_array(uint32_t processor_no);
volatile void thread_start(void);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* THREAD_THREAD_LOCAL_H */

