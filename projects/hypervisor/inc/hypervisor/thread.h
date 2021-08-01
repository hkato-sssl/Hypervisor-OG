/*
 * hypervisor/thread.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef HYPERVISOR_THREAD_H
#define HYPERVISOR_THREAD_H

/* includes */

/* defines */

#define TLS_SIZE                256

#define TLS_EXCEPTION_VECTOR    0
#define TLS_EXCEPTION_SP        1
#define TLS_EMERGENCY_SP        2
#define TLS_THREAD_SP           3
#define TLS_CURRENT_VPC_REGS    4
#define TLS_CURRENT_VPC         5
#define TLS_CURRENT_VM          6

/* types */
#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>
#include "lib/system/errno.h"

/* defines */

#define NR_THREAD_ARGS      8

/* types */

typedef void (*thread_entry_t)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t);

struct thread_parameter {
    thread_entry_t  entry;
    uintptr_t       args[NR_THREAD_ARGS];
};

/* variables */

/* functions */

errno_t thread_launch(uint32_t processor_no, const struct thread_parameter *parameter);
errno_t thread_launch_self(const struct thread_parameter *parameter);

uint64_t thread_read_tls(uint32_t index);
void thread_write_tls(uint32_t index, uint64_t d);
volatile void thread_terminate(void);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_THREAD_H */

