/*
 * driver/aarch64/mmu.h
 *
 * (C) 2019 Hidekazu Kato
 */

/*
 * 制約事項：
 * ・ARMv8仕様対応
 * ・4KB granuleのみ対応
 */

#ifndef DRIVER_AARCH64_MMU_H
#define DRIVER_AARCH64_MMU_H

/* includes */

/* defines */

#ifndef ASSEMBLY

/* includes */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "lib/bit.h"
#include "lib/list.h"
#include "lib/system/errno.h"

#ifdef __cplusplus
extern "C" {
#endif

/* defines */

/* Next-level descriptor attriutes */

#define MMU_DESC_NSTABLE            BIT(63)
#define MMU_DESC_APTABLE(n)         (((n) & 3ULL) << 61)
#define MMU_DESC_UXNTABLE           BIT(60)
#define MMU_DESC_XNTABLE            BIT(60)
#define MMU_DESC_PXNTABLE           BIT(59)

/*
 * Attribute fields for VMSAv8-64 stage 1 Block and
 * Page descriptors
 */

#define MMU_DESC_UXN                BIT(54)
#define MMU_DESC_PBHA(n)            (((n) & 0x0fULL) << 59)
#define MMU_DESC_XN                 BIT(54)
#define MMU_DESC_PXN                BIT(53)
#define MMU_DESC_CONTIGUOUS         BIT(52)

#define MMU_DESC_NG                 BIT(11)
#define MMU_DESC_AF                 BIT(10)
#define MMU_DESC_SH(n)              (((n) & 3ULL) << 8)
#define MMU_DESC_AP21(n)            (((n) & 3ULL) << 6)
#define MMU_DESC_NS                 BIT(5)
#define MMU_DESC_ATTRINDX(n)        (((n) & 7ULL) << 2)

#define MMU_ATTR_AP_RW              0   /* Read-Wirte, EL0 isn't accessible */
#define MMU_ATTR_AP_RW_EL0          1   /* Read-Write, EL0 is accessible */
#define MMU_ATTR_AP_RO              2   /* Read-Only, EL0 isn't accessible */
#define MMU_ATTR_AP_RO_EL0          3   /* Read-Only, EL0 is accessible */

#define MMU_ATTR_SH_NSH             0   /* Non-shareable */
#define MMU_ATTR_SH_OSH             2   /* Outer Shareable */
#define MMU_ATTR_SH_ISH             3   /* Inner Shareable */

#define MMU_TCR_SH_NSH              0   /* Non-shareable */
#define MMU_TCR_SH_OSH              2   /* Outer Shareable */
#define MMU_TCR_SH_ISH              3   /* Inner Shareable */

#define MMU_TCR_RGN_NC              0   /* Normal memory, Non-cacheable */
#define MMU_TCR_RGN_WBWA            1   /* Normal memory, Write-Back Write-Allocate Cacheable */
#define MMU_TCR_RGN_WT              2   /* Normal memory, Write-Through Cacheable */
#define MMU_TCR_RGN_WB              3   /* Normal memory, Write-Back no Write-Allocate Cacheable */

#define MMU_TCR_PS_4GB              0   /* 32 bits, 4GB */
#define MMU_TCR_PS_64GB             1   /* 36 bits, 64GB */
#define MMU_TCR_PS_1TB              2   /* 40 bits, 1TB */
#define MMU_TCR_PS_4TB              3   /* 42 bits, 4TB */
#define MMU_TCR_PS_16TB             4   /* 44 bits, 16TB */
#define MMU_TCR_PS_256TB            5   /* 48 bits, 256TB */

/* types */

enum aarch64_mmu_type { AARCH64_MMU_STAGE2, AARCH64_MMU_EL0, AARCH64_MMU_EL1, AARCH64_MMU_EL2, AARCH64_MMU_EL3 };
enum aarch64_mmu_granule { AARCH64_MMU_4KB_GRANULE, AARCH64_MMU_16KB_GRANULE, AARCH64_MMU_64KB_GRANULE }; 

/* Attributes in descriptor for stage 1 translation */

struct aarch64_mmu_attr {
    /* attributes in Table descriptor */

    uint8_t     nstable:1;
    uint8_t     aptable:2;
    uint8_t     xntable:1;  /* also used as UXNTable */
    uint8_t     pxntable:1;

    /* attrinbutes in Block or Page descriptor */

    /* upper attributes */
    uint8_t     xn:1;       /* also used as UXN */
    uint8_t     pxn:1;
    /* lower attributes */
    uint8_t     ng:1;
    uint8_t     af:1;
    uint8_t     sh:2;
    uint8_t     ap21:2;
    uint8_t     ns:1;
    uint8_t     attrindx:3;
};

/* Attributes in descriptor for stage 2 translation */

struct aarch64_mmu_stage2_attr {
    /* attrinbutes in Block or Page descriptor */

    /* upper attributes */
    uint8_t     xn:1;
    /* lower attributes */
    uint8_t     af:1;
    uint8_t     sh:2;
    uint8_t     s2ap:2;
    uint8_t     memattr:4;
};

struct aarch64_mmu_block_pool {
    size_t              block_sz;
    struct list         block_list;
    struct {
        void            *addr;
        size_t          size;
    } block_region;
    struct {
        struct {
            uint64_t    success;
            uint64_t    failure;
        } calloc;
        struct {
            uint64_t    success;
            uint64_t    failure;
        } free;
    } counter;
};

struct aarch64_mmu_block_pool_configuration {
    size_t          block_sz;
    struct {
        void        *addr;
        size_t      size;
    } block_region;
};

struct aarch64_mmu_tcr_el0 {
    uint32_t        sh0:2;
    uint32_t        orgn0:2;
    uint32_t        irgn0:2;
    uint32_t        t0sz:6;
};

struct aarch64_mmu_tcr_el1 {
    uint32_t        as:1;
    uint32_t        ips:3;
    uint32_t        sh1:2;
    uint32_t        orgn1:2;
    uint32_t        irgn1:2;
    uint32_t        a1:1;
    uint32_t        t1sz:6;
};

struct aarch64_mmu_tcr_el23 {
    uint32_t        ps:3;
    uint32_t        sh0:2;
    uint32_t        orgn0:2;
    uint32_t        irgn0:2;
    uint32_t        t0sz:6;
};

union aarch64_mmu_tcr {
    struct aarch64_mmu_tcr_el0 el0;
    struct aarch64_mmu_tcr_el1 el1;
    struct aarch64_mmu_tcr_el23 el23;
};

struct aarch64_mmu {
    bool active;
    enum aarch64_mmu_type type;
    enum aarch64_mmu_granule granule;

    struct {
        uint16_t    asid;
        uint64_t    mair;
        uint64_t    tcr;
    } stage1;

    struct {
        uint8_t     vmid;
        uint64_t    vtcr;
    } stage2;

    uint64_t        *addr;
    struct aarch64_mmu_block_pool pool;
};

struct aarch64_mmu_configuration {
    enum aarch64_mmu_type type;
    enum aarch64_mmu_granule granule;

    struct {
        uint16_t    asid;
        uint64_t    mair;
    } stage1;
    struct {
        uint8_t     vmid;
    } stage2;

    union aarch64_mmu_tcr tcr;
    struct aarch64_mmu_block_pool_configuration pool;
};

/* variables */

/* functions */

errno_t aarch64_mmu_init(struct aarch64_mmu *mmu, struct aarch64_mmu_configuration const *config);
errno_t aarch64_mmu_disable(void);
errno_t aarch64_mmu_enable(struct aarch64_mmu const *mmu);

errno_t aarch64_mmu_map(struct aarch64_mmu *mmu, void *va, void *pa, size_t sz, struct aarch64_mmu_attr const *attr);

errno_t aarch64_mmu_stage2_map(struct aarch64_mmu *mmu, void *va, void *pa, size_t sz, struct aarch64_mmu_stage2_attr const *attr);

errno_t aarch64_mmu_block_pool_init(struct aarch64_mmu_block_pool *pool, struct aarch64_mmu_block_pool_configuration const *config);
void *aarch64_mmu_block_calloc(struct aarch64_mmu_block_pool *pool, size_t block_sz);
errno_t aarch64_mmu_block_free(struct aarch64_mmu_block_pool *pool, void *block, size_t block_sz);

/* for debugging */

void aarch64_mmu_dump_descriptor(struct aarch64_mmu const *mmu);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_AARCH64_MMU_H */

