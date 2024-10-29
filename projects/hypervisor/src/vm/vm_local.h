// SPDX-License-Identifier: Apache-2.0
/*
 * vm/vm_local.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef VM_VM_LOCAL_H
#define VM_VM_LOCAL_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "lib/system/errno.h"
#include <stdint.h>

/* defines */

/* types */

/* variables */

/* functions */

errno_t vm_event_loop(struct vm *vm, uint32_t proc_no,
                      const struct vpc_boot_configuration *boot);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* VM_VM_LOCAL_H */
