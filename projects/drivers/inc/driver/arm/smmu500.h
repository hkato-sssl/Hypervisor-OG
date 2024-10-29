// SPDX-License-Identifier: Apache-2.0
/*
 * driver/arm/smmu500.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef DRIVER_ARM_SMMU500_H
#define DRIVER_ARM_SMMU500_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include "lib/bit.h"
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"
#include <stdint.h>

/* defines */

#define MAX_NR_SMMU_STREAM_MAPS         128
#define MAX_NR_SMMU_CONTEXT_BANKS       128

/* memory type */

#define SMMU_MT_DEVICE_nGnRnE           0x0
#define SMMU_MT_DEVICE_nGnRE            0x1
#define SMMU_MT_DEVICE_nGRE             0x2
#define SMMU_MT_DEVICE_GRE              0x3

#define SMMU_MT_NORMAL_ONC              (0x1 << 2)
#define SMMU_MT_NORMAL_OWT              (0x2 << 2)
#define SMMU_MT_NORMAL_OWB              (0x3 << 2)
#define SMMU_MT_NORMAL_INC              0x1
#define SMMU_MT_NORMAL_IWT              0x2
#define SMMU_MT_NORMAL_IWB              0x3
#define SMMU_MT_NORMAL_NC               (SMMU_MT_NORMAL_ONC | SMMU_MT_NORMAL_INC)
#define SMMU_MT_NORMAL_WT               (SMMU_MT_NORMAL_OWT | SMMU_MT_NORMAL_IWT)
#define SMMU_MT_NORMAL_WB               (SMMU_MT_NORMAL_OWB | SMMU_MT_NORMAL_IWB)

/* transient allocate configuration */

#define SMMU_TRANSIENTCFG_DEFAULT       0
#define SMMU_TRANSIENTCFG_NON_TRANSIENT 2
#define SMMU_TRANSIENTCFG_TRANSIENT     3

/* instruction fetch attribute configuration */

#define SMMU_INSTCFG_DEFAULT            0
#define SMMU_INSTCFG_DATA               2
#define SMMU_INSTCFG_INSTRUCTION        3

/* privileged attribute configutaion */

#define SMMU_PRIVCFG_DEFAULT            0
#define SMMU_PRIVCFG_PRIVILEGED_NEVER   1
#define SMMU_PRIVCFG_UNPRIVILEGED       2
#define SMMU_PRIVCFG_PRIVILEGED         3

/* allocate configuration */

#define SMMU_WACFG_DEFAULT              0
#define SMMU_WACFG_WRITE_ALLOCATE       2
#define SMMU_WACFG_NO_WRITE_ALLOCATE    3
#define SMMU_RACFG_DEFAULT              0
#define SMMU_RACFG_READ_ALLOCATE        2
#define SMMU_RACFG_NO_READ_ALLOCATE     3

/* shared configuration */

#define SMMU_SHCFG_OUTER_SHAREABLE      1
#define SMMU_SHCFG_INNER_SHAREABLE      2
#define SMMU_SHCFG_NON_SHAREABLE        3

/* barrier shareability upgrade */

#define SMMU_BSU_NO_EFFECT              0
#define SMMU_BSU_INNER_SHAREABLE        1
#define SMMU_BSU_OUTER_SHAREABLE        2
#define SMMU_BSU_FULL_SYSTEM            3

/* secure configuration */

#define SMMU_NSCFG_DEFAULT              0
#define SMMU_NSCFG_SECURE               2
#define SMMU_NSCFG_NON_SECURE           3

/* memory attribute */

#define SMMU_MTCFG_DEFAULT              0
#define SMMU_MTCFG_MEMATTR              1

/* types */

struct aarch64_mmu;
struct aarch64_mmu_attr;
struct aarch64_stage2;

struct smmu500 {
    spin_lock_t lock;
    uintptr_t smmu_base; /* also used as SMMU_GR0_BASE */
    uintptr_t smmu_gr1_base;
    uintptr_t smmu_cb_base;

    uint32_t nr_pages;
    uint32_t page_size; /* 4096 or 65536 */

    uint8_t nr_stream_matches;   /* SMMU_IDR0.SMRG */
    uint8_t nr_context_banks;    /* SMMU_IDR1.NUMCB */
    uint8_t nr_s2_context_banks; /* SMMU_IDR1.NUMS2CB */
    uint8_t nr_context_fault_interrupts;

    uint8_t vmid_size; /* 8 or 16 */

    struct {
        uint8_t stream_matches[MAX_NR_SMMU_STREAM_MAPS];
        uint8_t context_banks[MAX_NR_SMMU_CONTEXT_BANKS];
    } allocation;
};

struct smmu500_configuration {
    uintptr_t smmu_base;
    struct aarch64_mmu *mmu;
    struct aarch64_mmu_attr *mmu_attr;
    struct {
        uint8_t interrupt : 1; /* global interrupt */
        uint8_t fault     : 1;
    } flag;
};

struct smmu_stream {
    uint16_t mask;
    uint16_t id;
};

struct smmu_translation_stream_configuration {
    struct smmu_stream stream;

    uint32_t transientcfg : 2;
    uint32_t instcfg      : 2;
    uint32_t privcfg      : 2;
    uint32_t wacfg        : 2;
    uint32_t racfg        : 2;
    uint32_t nscfg        : 2;
    uint32_t mtcfg        : 1;
    uint32_t shcfg        : 2;

    struct {
        uint8_t exidvalid : 1;
    } flag;

    uint8_t memattr;
    uint8_t cbndx;
};

struct smmu_context_bank_with_stage2_configuration {
    struct aarch64_stage2 *stage2;
    uint16_t vmid;
    uint8_t interrupt_index;
    struct {
        uint8_t interrupt : 1;
        uint8_t fault     : 1;
    } flag;
};

/* variables */

/* functions */

errno_t smmu500_initialize(struct smmu500 *smmu,
                           const struct smmu500_configuration *config);
errno_t smmu500_create_context_bank_with_stage2(
    struct smmu500 *smmu, uint8_t *cb,
    const struct smmu_context_bank_with_stage2_configuration *config);
errno_t smmu500_create_translation_stream(
    struct smmu500 *smmu, uint8_t *id,
    const struct smmu_translation_stream_configuration *config);
errno_t smmu500_enable(struct smmu500 *smmu, uint8_t id);
errno_t smmu500_disable(struct smmu500 *smmu, uint8_t id);

/* for debugging */

void smmu500_dump(struct smmu500 *smmu);
void smmu500_dump_stream_match_register(struct smmu500 *smmu, uint8_t id);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_ARM_SMMU500_H */
