/*
 * aarch64/mmu/aarch64_mmu_dump_descriptor.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/printk.h"
#include "driver/aarch64/mmu.h"

/* defines */

/* types */

/* prototypes */

static void dump_table(struct aarch64_mmu_base const *mmu, uint64_t *p, int level);

/* variables */

/* functions */

static void indent(int level)
{
    int i;

    for (i = 0; i < level; ++i) {
        printk("   ");
    }
}

static void dump_2bits(uint64_t desc)
{
    char buff[8];

    buff[0] = (desc & BIT(1)) ? '1' : '0';
    buff[1] = (desc & BIT(0)) ? '1' : '0';
    buff[2] = 'b';
    buff[3] = '\0';
    printk(buff);
}

static void dump_invalid_desc(uint64_t desc, int level, int index, int invalid_ct)
{
    if (invalid_ct == 0) {
        indent(level);
        printk("%03x:%016llx\n", index, desc);
    } else if (invalid_ct == 1) {
        indent(level);
        printk("    ........\n");
    }
}

static void dump_block_page_attr(uint64_t desc)
{
    if (desc & MMU_DESC_XN) {
        printk(", UXN/XN");
    }
    if (desc & MMU_DESC_PXN) {
        printk(", PXN");
    }
    if (desc & MMU_DESC_CONTIGUOUS) {
        printk(", Contiguous");
    }
    if (desc & MMU_DESC_NG) {
        printk(", nG");
    }
    if (desc & MMU_DESC_AF) {
        printk(", AF");
    }
    printk(", SH=");
    dump_2bits(desc >> 8);
    printk(", AP[2:1]=");
    dump_2bits(desc >> 6);
    if (desc & MMU_DESC_NS) {
        printk(", NS");
    }
    printk(", AttrIndx=%x\n", BF_EXTRACT(desc, 4, 2));
}

static void dump_block_desc(uint64_t desc, int level, int index)
{
    uint64_t out;

    out = desc & BITS(47, 12);

    indent(level);
    printk("%03x:%016llx: OutAddr=%016llx", index, desc, out);
    dump_block_page_attr(desc);
}

static void dump_table_desc(struct aarch64_mmu_base const *mmu, uint64_t desc, int level, int index)
{
    uint64_t next;

    next = desc & BITS(47, 12);
    indent(level);
    printk("%03x:%016llx Next-level=%016llx", index, desc, next);
    if (desc & MMU_DESC_NSTABLE) {
        printk(", NSTable");
    }
    printk(", APTable=");
    dump_2bits(desc >> 61);
    if (desc & MMU_DESC_XNTABLE) {
        printk(", XNTable");
    }
    if (desc & MMU_DESC_PXNTABLE) {
        printk(", PXNTable");
    }
    printk("\n");
    dump_table(mmu, (uint64_t*)next, (level + 1));
}

static void dump_page_desc(uint64_t desc, int level, int index)
{
    uint64_t out;

    out = desc & BITS(47, 12);
    indent(level);
    printk("%03x:%016x: OutAddr=%016x", index, desc, out);
    dump_block_page_attr(desc);
}

static void dump_reserved_desc(uint64_t desc, int level, int index)
{
    indent(level);
    printk("%03x:%016x: reserved\n", index, desc);
}

static void dump_table(struct aarch64_mmu_base const *mmu, uint64_t *p, int level)
{
    int i;
    int invalid;
    uint64_t desc;

    invalid = 0;
    for (i = 0; i < 512; ++i) {
        desc = p[i];
        switch (desc & 3) {
        case 0:
        case 2:
            dump_invalid_desc(desc, level, i, invalid);
            ++invalid;
            break;
        case 1:
            if (level < 3) {
                dump_block_desc(desc, level, i);
            } else {
                dump_reserved_desc(desc, level, i);
            }
            invalid = 0;
            break;
        case 3:
            if (level < 3) {
                dump_table_desc(mmu, desc, level, i);
            } else {
                dump_page_desc(desc, level, i);
            }
            invalid = 0;
            break;
        }
    }
}

void aarch64_mmu_dump_descriptor(struct aarch64_mmu const *mmu)
{
    uint64_t *p;

    p = mmu->base.addr;
    printk("<TTBR:%016llx>\n", p);
    if (mmu->base.granule == AARCH64_MMU_4KB_GRANULE) {
        dump_table(&(mmu->base), p, 0);
    }
}

