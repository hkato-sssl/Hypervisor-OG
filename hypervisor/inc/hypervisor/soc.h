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
#include "hypervisor/mmu.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"

/* defines */

/* types */

struct soc;

struct soc_device_interrupt {
    uint16_t    virtual;
    uint16_t    physical;
}; 

struct soc_device_emulator {
    struct vm_region_trap   *trap;
    void                    *arg;
    vpc_emulator_t          handler;
};

struct soc_device_region {
    struct soc_device_emulator  *emulator;

    uintptr_t                   ipa;
    uintptr_t                   pa;
    size_t                      size;
    uint8_t                     memory_type;
    uint8_t                     shareability;
    struct {
        struct {
            uint8_t             read:1;
            uint8_t             write:1;
            uint8_t             exec:1;
        } flag;
    } access;
};

struct soc_device {
    uint16_t                    nr_irqs;
    struct soc_device_interrupt *irqs;

    uint16_t                    nr_regions;
    struct soc_device_region    **regions;
};

struct soc_ops {
    errno_t (*test_executable_region)(struct soc *, uintptr_t addr, size_t size);
};

struct soc {
    struct vm               vm;
    spin_lock_t             lock;
    void                    *chip;
    const struct soc_ops    *ops;
    uint16_t                nr_devices;
    struct soc_device       **devices;
};

struct soc_configuration {
    void                    *chip;
    const struct soc_ops    *ops;
    uint16_t                nr_procs;
    const struct aarch64_stage2_configuration   *stage2;
    uint16_t                nr_devices;
    struct soc_device       **devices;
};

/* variables */

/* functions */

errno_t soc_initialize(struct soc *soc, const struct soc_configuration *config);
errno_t soc_initialize_vpc(struct soc *soc, struct vpc *vpc, const struct vpc_configuration *config);
errno_t soc_default_test_executable_region(struct soc *soc, uintptr_t addr, size_t size);

/* inline functions */

static inline void soc_lock(struct soc *soc)
{
    spin_lock(&(soc->lock));
}

static inline void soc_unlock(struct soc *soc)
{
    spin_unlock(&(soc->lock));
}

static inline errno_t soc_test_executable_region(struct soc *soc, uintptr_t addr, size_t size)
{
    return (*(soc->ops->test_executable_region))(soc, addr, size);
}

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_SOC_H */

