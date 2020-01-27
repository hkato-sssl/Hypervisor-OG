/*
 * hypervisor/soc.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef HYPERVISOR_SOC_H
#define HYPERVISOR_SOC_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"
#include "driver/aarch64/stage2.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"

/* defines */

/* types */

struct soc {
    spin_lock_t             lock;
    struct vm               vm;
    struct aarch64_stage2   stage2;
};

/* variables */

/* functions */

/* inline functions */

static inline void soc_lock(struct soc *soc)
{
    spin_lock(&(soc->lock));
}

static inline void soc_unlock(struct soc *soc)
{
    spin_unlock(&(soc->lock));
}

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_SOC_H */

