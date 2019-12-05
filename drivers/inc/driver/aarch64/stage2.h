/*
 * driver/aarch64/stage2.h
 *
 * (C) 2019 Hidekazu Kato
 */

/*
 * 制約事項：
 * ・ARMv8仕様対応
 * ・4KB granuleのみ対応
 */

#ifndef DRIVER_AARCH64_STAGE2_H
#define DRIVER_AARCH64_STAGE2_H

/* includes */

/* defines */

#ifndef ASSEMBLY

/* includes */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "driver/aarch64/mmu_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/* defines */

/*
 * Attribute fields for VMSAv8-64 stage 2 Block and
 * Page descriptors
 */

#define STAGE2_DESC_XN                  BIT(54)
#define STAGE2_DESC_CONTIGUOUS          BIT(52)

#define STAGE2_DESC_AF                  BIT(10)
#define STAGE2_DESC_SH(n)               (((n) & 3ULL) << 8)
#define STAGE2_DESC_S2AP(n)             (((n) & 3ULL) << 6)
#define STAGE2_DESC_MEMATTR(n)          (((n) & 0x0fULL) << 2)

#define STAGE2_SH_NSH                   0   /* Non-shareable */
#define STAGE2_SH_OSH                   2   /* Outer Shareable */
#define STAGE2_SH_ISH                   3   /* Inner Shareable */

#define STAGE2_S2AP_NONE                0   /* None */
#define STAGE2_S2AP_RO                  1   /* Read-only */
#define STAGE2_S2AP_WO                  2   /* Write-only */
#define STAGE2_S2AP_RW                  3   /* Read/write */

#define STAGE2_MEMATTR_DEVICE_nGnRnE    0
#define STAGE2_MEMATTR_DEVICE_nGnRE     1
#define STAGE2_MEMATTR_DEVICE_nGRE      2
#define STAGE2_MEMATTR_DEVICE_GRE       3

#define STAGE2_MEMATTR_NORMAL_ONC       (1 << 2)   /* Outer Non-cacheable */
#define STAGE2_MEMATTR_NORMAL_OWT       (2 << 2)   /* Outer Write-Through Cacheable */
#define STAGE2_MEMATTR_NORMAL_OWB       (3 << 2)   /* Outer Write-Back Cacheable */

#define STAGE2_MEMATTR_NORMAL_INC       1          /* Inner Non-cacheable */
#define STAGE2_MEMATTR_NORMAL_IWT       2          /* Inner Write-Through Cacheable */
#define STAGE2_MEMATTR_NORMAL_IWB       3          /* Inner Write-Back Cacheable */

#define STAGE2_MEMATTR_NORMAL_NC        (STAGE2_MEMATTR_NORMAL_ONC | STAGE2_MEMATTR_NORMAL_INC)
#define STAGE2_MEMATTR_NORMAL_WT        (STAGE2_MEMATTR_NORMAL_OWT | STAGE2_MEMATTR_NORMAL_IWT)
#define STAGE2_MEMATTR_NORMAL_WB        (STAGE2_MEMATTR_NORMAL_OWB | STAGE2_MEMATTR_NORMAL_IWB)

/* types */

/* Attributes in descriptor for stage 2 translation */

struct aarch64_stage2_attr {
    /* attrinbutes in Block or Page descriptor */

    /* upper attributes */
    uint8_t     xn:1;
    /* lower attributes */
    uint8_t     af:1;
    uint8_t     sh:2;
    uint8_t     s2ap:2;
    uint8_t     memattr:4;
};

struct aarch64_stage2_vtcr_el2 {
    uint32_t    ps:3;
    uint32_t    sh0:2;
    uint32_t    orgn0:2;
    uint32_t    irgn0:2;
    uint32_t    t0sz:6;
};

struct aarch64_stage2 {
    struct aarch64_mmu_base base;

    uint16_t    vmid;
    uint64_t    vtcr_el2;
};

struct aarch64_stage2_configuration {
    struct aarch64_mmu_base_configuration base;

    uint16_t    vmid;

    struct aarch64_stage2_vtcr_el2 vtcr_el2;
};

/* variables */

/* functions */

errno_t aarch64_stage2_init(struct aarch64_stage2 *st2, struct aarch64_stage2_configuration const *config);
errno_t aarch64_stage2_map(struct aarch64_stage2 *st2, void *ipa, void *pa, size_t sz, struct aarch64_stage2_attr const *attr);
errno_t aarch64_stage2_activate(struct aarch64_stage2 *st2);

/* for debugging */

void aarch64_stage2_dump_descriptor(struct aarch64_stage2 const *st2);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_AARCH64_STAGE2_H */

