/*
 * hypervisor/soc/xilinx/mpsoc.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef HYPERVISOR_SOC_XILINX_MPSOC_H
#define HYPERVISOR_SOC_XILINX_MPSOC_H

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
#include "driver/arm/gic400.h"
#include "driver/xilinx/mpsoc.h"
#include "hypervisor/emulator/vgic400.h"
#include "hypervisor/soc.h"

/* defines */

#define DESC_XILINX_MPSOC_STAGE2_LEVEL1_TABLE(name)   uint8_t name[4096 * 2] __attribute__ ((aligned(4096 * 2)))

/* types */

struct xilinx_mpsoc {
    struct soc      soc;

    struct vgic400  vgic400;
};

struct xilinx_mpsoc_configuration {
    uint8_t         nr_procs;
    uint16_t        vmid;

    struct {
        struct aarch64_mmu_block_pool   *pool;
        void                            *level1_table;
    } stage2;

    struct gic400   *gic;

    struct {
        uintptr_t   ipa;
        uintptr_t   pa;
        size_t      size;
    } ram;
};

/* variables */

/* functions */

errno_t xilinx_mpsoc_initialize(struct xilinx_mpsoc *soc, const struct xilinx_mpsoc_configuration *config);
errno_t xilinx_mpsoc_initialize_vpc(struct xilinx_mpsoc *soc, struct vpc *vpc, const struct vpc_configuration *config);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_SOC_XILINX_MPSOC_H */

