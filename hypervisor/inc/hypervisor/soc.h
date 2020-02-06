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

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"
#include "driver/aarch64/stage2.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"

/* defines */

/* types */

struct soc;

struct soc_ops {
    bool (*test_executable_region)(struct soc *soc, uintptr_t addr);
};

struct soc {
    spin_lock_t             lock;
    void                    *chip;
    const struct soc_ops    *ops;
    struct vm               vm;
    struct aarch64_stage2   stage2;
};

struct soc_configuration {
    void                    *chip;
    const struct soc_ops    *ops;
    uint16_t                nr_procs;
};

/* variables */

/* functions */

errno_t soc_initialize(struct soc *soc, const struct soc_configuration *config);
errno_t soc_initialize_vpc(struct soc *soc, struct vpc *vpc, const struct vpc_configuration *config);

/* inline functions */

static inline void soc_lock(struct soc *soc)
{
    spin_lock(&(soc->lock));
}

static inline void soc_unlock(struct soc *soc)
{
    spin_unlock(&(soc->lock));
}

static inline bool soc_test_executable_region(struct soc *soc, uintptr_t addr)
{
    return (*(soc->ops->test_executable_region))(soc->chip, addr);
}

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_SOC_H */

