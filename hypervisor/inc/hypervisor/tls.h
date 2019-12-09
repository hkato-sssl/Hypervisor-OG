/*
 * hypervisor/tls.h - definitions of Thread Local Storage
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef HYPERVISOR_TLS_H
#define HYPERVISOR_TLS_H

/* includes */

/* defines */

#define TLS_SIZE                256

#define TLS_EXCEPTION_SP        0
#define TLS_CURRENT_VPC_REGS    1
#define TLS_CURRENT_VPC         2
#define TLS_CURRENT_VM          3

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

uintptr_t tls_read(uint32_t index);
void tls_write(uint32_t index, uintptr_t value);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_TLS_H */

