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
#include "hypervisor/vpc.h"

/* defines */

#define NR_XILINX_MPSOC_CPUS    4

#define DESC_XILINX_MPSOC_STAGE2_LEVEL1_TABLE(name)   uint8_t name[4096 * 2] __attribute__ ((aligned(4096 * 2)))

#define NR_SMMU_STREAM_IDS      (256 / sizeof(uint64_t))

/* types */

struct gic400;
struct smmu500;

struct xilinx_mpsoc {
    struct soc              soc;
    struct vgic400          vgic400;

    struct {
        struct smmu500      *device;
        uint8_t             context_bank;
        uint64_t            stream_ids[NR_SMMU_STREAM_IDS];
    } smmu;

    struct {
        uintptr_t           pa;
        uintptr_t           ipa;
        size_t              size;
    } ram;
};

struct xilinx_mpsoc_configuration {
    uint16_t            vmid;
    uint8_t             nr_procs;

    struct {
        struct vpc                      **vpcs;
        uint64_t                        **register_arrays;
        const struct vpc_hook           *hook;
        const struct vpc_exception_ops  *ops;
    } vpc;

    struct {
        struct aarch64_mmu_block_pool   *pool;
        void                            *level1_table;
    } stage2;

    struct gic400       *gic;

    struct {
        struct smmu500              *device;
        uint32_t                    nr_streams;
        const struct smmu_stream    *streams;
    } smmu;

    struct {
        uintptr_t       pa;
        uintptr_t       ipa;
        size_t          size;
    } ram;

    uint32_t            nr_devices;
    struct soc_device   *devices;

    uint16_t            nr_ppis;
    struct {
        uint16_t        virtual_id;
        uint16_t        physical_id;
    } ppis[NR_GIC400_PPIS];
};

/* variables */

/* functions */

errno_t xilinx_mpsoc_initialize(struct xilinx_mpsoc *chip, const struct xilinx_mpsoc_configuration *config);
const struct vpc_exception_ops *xilinx_mpsoc_default_vpc_exception_ops(void);
const struct vpc_hook *xilinx_mpsoc_default_vpc_hook(void);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_SOC_XILINX_MPSOC_H */

