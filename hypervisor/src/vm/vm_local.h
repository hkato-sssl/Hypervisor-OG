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

#include <stdint.h>
#include "lib/system/errno.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"

/* defines */

/* types */

/* variables */

/* functions */

errno_t vm_event_loop(struct vm *vm, uint32_t proc_no, struct vpc_boot_configuration const *boot);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* VM_VM_LOCAL_H */

