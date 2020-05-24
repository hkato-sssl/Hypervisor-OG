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

#define SOC_DEVICE_NO_IRQ               0xffff

/* types */

struct soc;

enum soc_shareability {
    SOC_NSH,            /* None-shareabile */
    SOC_ISH,            /* Inner Shareable */
    SOC_OSH             /* Outer Shareable */
};

struct soc_device {
    uint16_t                nr_irqs;
    uint16_t                *irqs;

    struct {
        uintptr_t               pa;
        uintptr_t               ipa;
        size_t                  size;
        uint8_t                 memory_type;    /* hypervisor/mmu.h: HYP_MMU_MT_XXX */
        enum soc_shareability   shareability;
        struct {
            struct {
                uint8_t         read:1;
                uint8_t         write:1;
                uint8_t         exec:1;
            } flag;
        } access;
    } region;
};

struct soc_ops {
    errno_t (*test_executable_region)(struct vpc *vpc, uintptr_t addr);
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
errno_t soc_default_test_executable_region(struct vpc *vpc, uintptr_t addr);

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

