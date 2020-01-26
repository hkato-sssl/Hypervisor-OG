/*
 * thread/thread_local.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef THREAD_THREAD_LOCAL_H
#define THREAD_THREAD_LOCAL_H

/* includes */

#include "hypervisor/vm.h"

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* defines */

/* types */

/* variables */

extern thread_request_array[MAX_NR_VM_PROCESSORS][2];

/* functions */

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* THREAD_THREAD_LOCAL_H */

