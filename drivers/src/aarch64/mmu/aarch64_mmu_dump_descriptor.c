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

static void dump_table(struct aarch64_mmu const *mmu, uint64_t *p, int level);

/* variables */

/* functions */

static void indent(int level)
{
    int i;

    for (i = 0; i < level; ++i) {
        printk("  ");
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

static void dump_invalid_desc(uint64_t desc, int level)
{
    indent(level);
    printk("%016llx*\n", desc);
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
    printk(", AttrIndx=%u\n", BF_EXTRACT(desc, 4, 2));
}

static void dump_block_desc(uint64_t desc, int level)
{
    uint64_t out;

    out = desc & BITS(47, 12);

    indent(level);
    printk("%016llx -> %016llx", desc, out);
    dump_block_page_attr(desc);
}

static void dump_table_desc(struct aarch64_mmu const *mmu, uint64_t desc, int level)
{
    uint64_t next;

    next = desc & BITS(47, 12);
    indent(level);
    printk("%016llx: Next-level=%016llx", desc, next);
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

static void dump_page_desc(uint64_t desc, int level)
{
    uint64_t out;

    out = desc & BITS(47, 12);
    indent(level);
    printk("%016x -> %016x", desc, out);
    dump_block_page_attr(desc);
}

static void dump_table(struct aarch64_mmu const *mmu, uint64_t *p, int level)
{
    int i;
    uint64_t desc;

    for (i = 0; i < 512; ++i) {
        desc = p[i];
        switch (desc & 3) {
            case 0:
                //dump_invalid_desc(desc, level);
                break;
            case 1:
                dump_block_desc(desc, level);
                break;
            case 2:
            case 3:
                if (level < 3) {
                    dump_table_desc(mmu, desc, level);
                } else {
                    dump_page_desc(desc, level);
                }
                break;
        }
    }
}

void aarch64_mmu_dump_descriptor(struct aarch64_mmu const *mmu)
{
    uint64_t *p;

    p = mmu->addr;
    printk("<%p>\n", p);
    if (mmu->granule == AARCH64_MMU_4KB_GRANULE) {
        dump_table(mmu, p, 0);
    }
}

