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

#include <stdint.h>
#include "lib/system/errno.h"
#include "lib/system/spin_lock.h"
#include "lib/bit.h"

/* defines */

#define MAX_NR_SMMU_CONTEXTS        128

/* memory type */

#define SMMU_MT_DEVICE_nGnRnE       0x0
#define SMMU_MT_DEVICE_nGnRE        0x1
#define SMMU_MT_DEVICE_nGRE         0x2
#define SMMU_MT_DEVICE_GRE          0x3

#define SMMU_MT_NORMAL_ONC          (0x1 << 2)
#define SMMU_MT_NORMAL_OWT          (0x2 << 2)
#define SMMU_MT_NORMAL_OWB          (0x3 << 2)
#define SMMU_MT_NORMAL_INC          0x1
#define SMMU_MT_NORMAL_IWT          0x2
#define SMMU_MT_NORMAL_IWB          0x3
#define SMMU_MT_NORMAL_NC           (SMMU_MT_NORMAL_ONC | SMMU_MT_NORMAL_INC)
#define SMMU_MT_NORMAL_WT           (SMMU_MT_NORMAL_OWT | SMMU_MT_NORMAL_IWT)
#define SMMU_MT_NORMAL_WB           (SMMU_MT_NORMAL_OWB | SMMU_MT_NORMAL_IWB)

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

/* types */

struct aarch64_mmu;
struct aatch64_mmu_attr;
struct aatch64_stage2;

struct smmu500 {
    spin_lock_t lock;
    uintptr_t   smmu_base;              /* also used as SMMU_GR0_BASE */
    uintptr_t   smmu_gr1_base;
    uintptr_t   smmu_cb_base;

    uint32_t    nr_pages;
    uint32_t    page_size;              /* 4096 or 65536 */

    uint8_t     nr_stream_maps;         /* SMMU_IDR0.SMRG */
    uint8_t     nr_context_banks;       /* SMMU_IDR1.NUMCB */
    uint8_t     nr_s2_context_banks;    /* SMMU_IDR1.NUMS2CB */

    struct {
        uint32_t    stream_maps[ALIGN(MAX_NR_SMMU_CONTEXTS, 32)];
        uint32_t    context_banks[ALIGN(MAX_NR_SMMU_CONTEXTS, 32)];
    } allocate;
};

struct smmu_s2_cb_translation_configuration {
    struct {
        uint16_t    transient:1;
        uint16_t    instruction:1;
        uint16_t    unprivileged:1;
        uint16_t    write_allocate:1;
        uint16_t    read_allocate:1;
        uint16_t    memory_attribute:1;
        uint16_t    extended_id:1;
        uint16_t    shareability:1;
    } flag;

    uint8_t         memory_attribute;
    uint8_t         shareability;
};

struct smmu500_configuration {
    uintptr_t               smmu_base;
    struct aarch64_mmu      *mmu;
    struct aarch64_mmu_attr *mmu_attr;
};

struct smmu_s2_cb_sctlr {
    uint32_t    wacfg:2;
    uint32_t    racfg:2;
    uint32_t    shcfg:2;
    uint32_t    fb:1;
    uint32_t    memattr:4;
    uint32_t    bsu:2;
    uint32_t    ptw:1;
    uint32_t    hupcf:1;
    uint32_t    cfcfg:1;
    uint32_t    cfie:1;
    uint32_t    cfre:1;
    uint32_t    e:1;
    uint32_t    affd:1;
    uint32_t    afe:1;
    uint32_t    tre:1;
    uint32_t    m:1;
};

struct smmu_s2_cb_tcr {
    uint32_t    hd:1;
    uint32_t    ha:1;
    uint32_t    pasize:3;
    uint32_t    tg0:2;
    uint32_t    sh0:2;
    uint32_t    orgn0:2;
    uint32_t    irgn0:2;
    uint32_t    sl0:2;
    uint32_t    t0sz:6;
};

struct smmu500_s2_cb_actlr {
    uint32_t    cpre:1;
    uint32_t    cmtlb:1;
};

struct smmu_stream {
    uint16_t    mask;
    uint16_t    id;
};

struct smmu500_s2_cb_attach_configuration {
    uint16_t                    vmid;
    struct smmu_stream          stream;
    const struct aarch64_stage2 *stage2;
    struct smmu_s2_cb_translation_configuration translation;
};

/* variables */

/* functions */

errno_t smmu500_initialize(struct smmu500 *smmu, const struct smmu500_configuration *config);
errno_t smmu500_s2_cb_attach(uint8_t *id, struct smmu500 *smmu, const struct smmu500_s2_cb_attach_configuration *config);

void smmu500_dump(const struct smmu500 *smmu);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_ARM_SMMU500_H */

