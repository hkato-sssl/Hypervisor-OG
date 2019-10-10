/*
 * hypervisor/vm.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef HYPERVISOR_VM_H
#define HYPERVISOR_VM_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* defines */

/* types */

struct vm {
	uint64_t	*regs;
};

/* variables */

/* functions */

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_VM_H */

