/*
 * driver/aarch64/mmu.h
 *
 * (C) 2019 Hidekazu Kato
 */

/*
 * 制約事項：
 * 現状ではARMv8仕様にのみ対応し、マッピング可能な単位は4KBのみである。
 */

#ifndef DRIVER_AARCH64_MMU_H
#define DRIVER_AARCH64_MMU_H

/* includes */

#include "lib/bit.h"

/* defines */

#define NR_MAIR_ATTRS               8           /* number of attribute in MAIR_ELx */

/* Device memory type */
#define MEM_ATTR_DEVICE_nGnRnE      0x00
#define MEM_ATTR_DEVICE_nGnRE       0x04
#define MEM_ATTR_DEVICE_nGRE       .0x08
#define MEM_ATTR_DEVICE_GRE         0x0c

/* Inner cacheability for Normal memory */
#define MEM_ATTR_NORMAL_INC         0x04        /* Non-cacheable */
#define MEM_ATTR_NORMAL_IWTWA       0x09        /* Write-Through, Write-Allocate */
#define MEM_ATTR_NORMAL_IWTRA       0x0a        /* Write-Through, Read-Allocate */
#define MEM_ATTR_NORMAL_IWTRAWA     0x0b        /* Write-Through, Read-Allocate, Write-Allocate */
#define MEM_ATTR_NORMAL_IWBWA       0x0d        /* Write-Back, Wite-Allocate */
#define MEM_ATTR_NORMAL_IWBRA       0x0e        /* Write-Back, Read-Allocate */
#define MEM_ATTR_NORMAL_IWBRAWA     0x0f        /* Write-Back, Read-Allocate, Wite-Allocate */

/* Outer cacheability for Normal memory */
#define MEM_ATTR_NORMAL_ONC         0x40        /* Non-cacheable */
#define MEM_ATTR_NORMAL_OWTWA       0x90        /* Write-Through, Write-Allocate */
#define MEM_ATTR_NORMAL_OWTRA       0xa0        /* Write-Through, Read-Allocate */
#define MEM_ATTR_NORMAL_OWTRAWA     0xb0        /* Write-Through, Read-Allocate, Write-Allocate */
#define MEM_ATTR_NORMAL_OWBWA       0xd0        /* Write-Back, Wite-Allocate */
#define MEM_ATTR_NORMAL_OWBRA       0xe0        /* Write-Back, Read-Allocate */

#define MEM_ATTR_NORMAL_NC          (MEM_ATTR_NORMAL_ONC | MEM_ATTR_NORMAL_INC)
#define MEM_ATTR_NORMAL_WT          (MEM_ATTR_NORMAL_OWTRAWA | MEM_ATTR_NORMAL_IWTRAWA)
#define MEM_ATTR_NORMAL_WB          (MEM_ATTR_NORMAL_OWBRAWA | MEM_ATTR_NORMAL_IWBRAWA)

/* Next-level descriptor attriutes */

#define MMU_DESC_NSTABLE        BIT(63)
#define MMU_DESC_APTABLE(n)     (((n) & 3ULL) << 61)
#define MMU_DESC_UXNTABLE       BIT(60)
#define MMU_DESC_XNTABLE        BIT(60)
#define MMU_DESC_PNTABLE        BIT(59)

/*
 * Attribute fields for VMSAv8-64 stage 1 Block and
 * Page descriptors
 */

#define MMU_DESC_UXN            BIT(54)
#define MMU_DESC_PBHA(n)        (((n) & 0x0fULL) << 59)
#define MMU_DESC_XN             BIT(54)
#define MMU_DESC_PXN            BIT(53)
#define MMU_DESC_CONTIGUOUS     BIT(52)
#define MMU_DESC_DBM            BIT(51)

#define MMU_DESC_NT             BIT(16)
#define MMU_DESC_OA(n)          (((n) & 0x0fULL) << 12)
#define MMU_DESC_NG             BIT(11)
#define MMU_DESC_AF             BIT(10)
#define MMU_DESC_SH(n)          (((n) & 3ULL) << 8)
#define MMU_DESC_AP(n)          (((n) & 3ULL) << 6)
#define MMU_DESC_ATTRINDX(n)    (((n) & 7ULL) << 2)

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>

/* defines */

/* types */

#define MMU_ATTR_AP_RW          0   /* Read-Wirte, EL0 isn't accessible */
#define MMU_ATTR_AP_RW_EL0      1   /* Read-Write, EL0 is accessible */
#define MMU_ATTR_AP_RO          2   /* Read-Only, EL0 isn't accessible */
#define MMU_ATTR_AP_RO_EL0      3   /* Read-Only, EL0 is accessible */

#define MMU_ATTR_SH_NSH         0   /* Non-shareable */
#define MMU_ATTR_SH_OSH         2   /* Outer Shareable */
#define MMU_ATTR_SH_ISH         3   /* Inner Shareable */

#define MMU_TCR_SH_NSH          0   /* Non-shareable */
#define MMU_TCR_SH_OSH          2   /* Outer Shareable */
#define MMU_TCR_SH_ISH          3   /* Inner Shareable */

#define MMU_TCR_RGN_NC          0   /* Normal memory, Non-cacheable */
#define MMU_TCR_RGN_WBWA        1   /* Normal memory, Write-Back Write-Allocate Cacheable */
#define MMU_TCR_RGN_WT          2   /* Normal memory, Write-Through Cacheable */
#define MMU_TCR_RGN_WB          3   /* Normal memory, Write-Back no Write-Allocate Cacheable */

struct aarch64_mmu_attr {
    /* upper attributes */
    uint8_t         xn:1;       /* also used as UXN */
    uint8_t         pxn:1;
    uint8_t         contiguous:1;
    /* lower attributes */
    uint8_t         ng:1;
    uint8_t         sh:2;
    uint8_t         ap:2;
    uint8_t         ns:1;
    uint8_t         attrindx:3;
};

struct aarch64_mmu_trans_table {
    uint64_t        *addr;
    uint16_t        asid;
    struct {
        uint8_t     sz:6;
        uint8_t     sh:2;
        uint8_t     irgn:2;
        uint8_t     orgn:2;
    } tcr;
};

/* variables */

/* functions */

errno_t aarch64_mmu_set_mair(uint8_t const *attributes);
errno_t aarch64_mmu_set_ttbr0(struct aarch64_mmu_trans_table const *tt);
errno_t aarch64_mmu_map(struct aarch64_mmu_trans_table *tt, void *va, void *pa, size_t sz, struct aarch64_mmu_attr const *attr);
errno_t aarch64_mmu_map_4KB(struct aarch64_mmu_trans_table *tt, void *va, void *pa, struct aarch64_mmu_attr const *attr);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_AARCH64_MMU_H */

