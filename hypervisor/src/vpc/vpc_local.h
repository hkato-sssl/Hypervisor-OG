/*
 * vpc_local.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef VPC_LOCAL_H
#define VPC_LOCAL_H

/* includes */

#include <stdint.h>
#include <stdbool.h>
#include "lib/bit.h"
#include "driver/aarch64/system_register/esr_el2_iss.h"
#include "hypervisor/vpc.h"

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* defines */

/* types */

/* variables */

/* functions */

errno_t vpc_switch_to_el1(uint64_t *regs);

/* inline functions */

static inline bool is_aarch64(const struct vpc *vpc)
{
    /* test M[4] */
    return ((vpc->regs[VPC_SPSR_EL2] & BIT(4)) == 0) ? true : false;
}

static inline bool is_aarch32(const struct vpc *vpc)
{
    return (! is_aarch64(vpc));
}

static inline bool is_aarch32a(const struct vpc *vpc)
{
    /* test T and M[4] */
    return ((vpc->regs[VPC_SPSR_EL2] & BITS(5, 4)) == BIT(4)) ? true : false;
}

static inline bool is_aarch32t(const struct vpc *vpc)
{
    /* test T and M[4] */
    return ((vpc->regs[VPC_SPSR_EL2] & BITS(5, 4)) == BITS(5, 4)) ? true : false;
}

static inline bool is_el0(const struct vpc *vpc)
{
    /* test T and M[3:0] */
    return ((vpc->regs[VPC_SPSR_EL2] & BITS(3, 0)) == 0) ? true : false;
}

static inline bool is_el1(const struct vpc *vpc)
{
    return (! is_el0(vpc));
}

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* VPC_LOCAL_H */

