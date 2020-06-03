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

/* types */

/* variables */

/* functions */

errno_t thread_launch(void *entry, void *arg);
errno_t thread_launch_at(uint32_t processor_no, void *entry, void *arg);
volatile void thread_start(void *entry, void *arg);

uint64_t thread_read_tls(uint32_t index);
void thread_write_tls(uint32_t index, uint64_t d);
volatile void thread_terminate(void);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_THREAD_H */

