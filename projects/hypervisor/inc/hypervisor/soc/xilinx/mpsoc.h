// SPDX-License-Identifier: Apache-2.0
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

#include "driver/arm/gic400.h"
#include "driver/xilinx/mpsoc.h"
#include "hypervisor/emulator/vgic400.h"
#include "hypervisor/soc.h"
#include "hypervisor/vpc.h"
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"
#include <stdint.h>

/* defines */

#define NR_XILINX_MPSOC_CPUS         4
#define XILINX_MPSOC_HVC_SERVICE_IMM 0x10

#define NR_P128_CONNECTIONS          1

#define DESC_XILINX_MPSOC_STAGE2_LEVEL1_TABLE(name) \
    uint8_t name[4096 * 2] __attribute__((aligned(4096 * 2)))

#define NR_SMMU_STREAM_IDS (256 / sizeof(uint64_t))

/* types */

struct aarch64_mmu;
struct gic400;
struct smmu500;
struct hvc_service;

struct xilinx_mpsoc_hvc_service {
    struct slist node;
    uint32_t id;
    vpc_exception_emulator_t handler;
};

struct xilinx_mpsoc {
    struct soc soc;
    struct vgic400 vgic400;

    struct {
        struct smmu500 *device;
        uint8_t context_bank;
        uint64_t stream_ids[NR_SMMU_STREAM_IDS];
    } smmu;

    struct slist hvc_service_list;
};

struct xilinx_mpsoc_configuration {
    uint16_t vmid;
    uint8_t nr_procs;

    struct {
        struct vpc *vpcs[NR_XILINX_MPSOC_CPUS];
        uint64_t *register_arrays[NR_XILINX_MPSOC_CPUS];
        const struct vpc_hook *hook;
        const struct vpc_exception_ops *ops;
    } vpc;

    struct aarch64_mmu *mmu;

    struct {
        struct aarch64_mmu_block_pool *pool;
        void *level1_table;
    } stage2;

    struct {
        struct gic400 *device;
        uint16_t nr_sgis;
        uint16_t sgis[NR_GIC400_SGIS];
        uint16_t nr_ppis;
        uint16_t ppis[NR_GIC400_PPIS];
        const struct vgic400_ops *ops;
        struct {
            bool half_priority;
        } boolean;
        struct vgic400_interrupt_event_array *event_arrays[NR_VGIC400_CPUS];
    } gic;

    struct {
        struct smmu500 *device;
        uint32_t nr_streams;
        const struct smmu_stream **streams;
        struct {
            uint8_t fault : 1;
        } flag;
    } smmu;

    uint32_t nr_devices;
    struct soc_device **devices;

    struct {
        uint32_t nr_services;
        struct hvc_service **services;
    } hvc;
};

/* variables */

/* functions */

errno_t
xilinx_mpsoc_initialize(struct xilinx_mpsoc *chip,
                        const struct xilinx_mpsoc_configuration *config);
const struct vpc_exception_ops *xilinx_mpsoc_default_vpc_exception_ops(void);
const struct vpc_hook *xilinx_mpsoc_default_vpc_hook(void);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_SOC_XILINX_MPSOC_H */
