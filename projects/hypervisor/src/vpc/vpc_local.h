// SPDX-License-Identifier: Apache-2.0
/*
 * vpc_local.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef VPC_LOCAL_H
#define VPC_LOCAL_H

/* includes */

#include "driver/aarch64.h"
#include "driver/aarch64/system_register/esr_el2_iss.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "lib/bit.h"
#include "lib/system/spin_lock.h"
#include <stdbool.h>
#include <stdint.h>

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
void vpc_set_boot_parameters(struct vpc *vpc,
                             const struct vpc_boot_configuration *boot);

/* inline functions */

static inline void vpc_lock(struct vpc *vpc)
{
    spin_lock(&(vpc->lock));
}

static inline void vpc_unlock(struct vpc *vpc)
{
    spin_unlock(&(vpc->lock));
}

static inline void vpc_set_status(struct vpc *vpc, enum vpc_status status)
{
    aarch64_dmb_ish();
    vpc->status = status;
    aarch64_dsb_ish();
}

static inline enum vpc_status vpc_watch_status(const struct vpc *vpc)
{
    return ((volatile const struct vpc *)vpc)->status;
}

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
    return ((vpc->regs[VPC_SPSR_EL2] & BITS(5, 4)) == BITS(5, 4)) ? true
                                                                  : false;
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

static inline errno_t validate_vpc(const struct vpc *vpc)
{
    errno_t ret;

    if (vpc->vm == NULL) {
        ret = -EINVAL;
    } else if (vpc->proc_no >= vpc->vm->nr_procs) {
        ret = -EINVAL;
    } else if (vpc->vm->vpcs[vpc->proc_no] != vpc) {
        ret = -EINVAL;
    } else {
        ret = SUCCESS;
    }

    return ret;
}

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* VPC_LOCAL_H */
