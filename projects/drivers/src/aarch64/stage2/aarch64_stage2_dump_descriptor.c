/*
 * aarch64/stage2/aarch64_stage2_dump_descriptor.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/printk.h"
#include "driver/aarch64/stage2.h"

/* defines */

/* types */

/* prototypes */

static void dump_table(struct aarch64_stage2 *stage2, uint64_t *p, int level);

/* variables */

/* functions */

static void indent(struct aarch64_stage2 const *stage2, int level)
{
    int i;

    for (i = stage2->base.start_level; i < level; ++i) {
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

static void dump_block_page_attr(uint64_t desc)
{
    if (desc & STAGE2_DESC_XN) {
        printk(", XN");
    }
    if (desc & STAGE2_DESC_CONTIGUOUS) {
        printk(", Contiguous");
    }
    if (desc & STAGE2_DESC_AF) {
        printk(", AF");
    }
    printk(", SH=");
    dump_2bits(desc >> 8);
    printk(", S2AP=");
    dump_2bits(desc >> 6);
    printk(", MemAttr=%x\n", BF_EXTRACT(desc, 4, 2));
}

static void dump_block_desc(struct aarch64_stage2 *stage2, uint64_t desc, int level)
{
    uint64_t out;

    out = desc & BITS(47, 12);

    printk("%016llx: OutAddr=%016llx", desc, out);
    dump_block_page_attr(desc);
}

static void dump_table_desc(struct aarch64_stage2 *stage2, uint64_t desc, int level)
{
    uint64_t next;

    next = desc & BITS(47, 12);
    printk("%016llx Next-level=%016llx", desc, next);
    printk("\n");
    dump_table(stage2, (uint64_t*)next, (level + 1));
}

static void dump_page_desc(struct aarch64_stage2 *stage2, uint64_t desc, int level)
{
    uint64_t out;

    out = desc & BITS(47, 12);
    printk("%016x: OutAddr=%016x", desc, out);
    dump_block_page_attr(desc);
}

static void dump_reserved_desc(struct aarch64_stage2 *stage2, uint64_t desc, int level)
{
    printk("%016x: reserved\n", desc);
}

static void dump_table(struct aarch64_stage2 *stage2, uint64_t *p, int level)
{
    int i;
    int n;
    int invalid;
    const char *fmt;
    uint64_t desc;

    if (stage2->base.start_level == level) {
        n = 1 << (stage2->pa_width - ((3 - level) * 9 + 12));
    } else {
        n = 512;
    }

    fmt = (n > 512) ? "%04x:" : "%03x:";

    invalid = 0;
    for (i = 0; i < n; ++i) {
        desc = p[i];
        switch (desc & 3) {
        case 0:
        case 2:
            if (invalid == 0) {
                indent(stage2, level);
                printk(fmt, i);
                printk("%016llx\n", desc);
            } else if (invalid == 1) {
                indent(stage2, level);
                printk(fmt, i);
                printk("....\n");
            }
            ++invalid;
            break;
        case 1:
            indent(stage2, level);
            printk(fmt, i);
            if (level < 3) {
                dump_block_desc(stage2, desc, level);
            } else {
                dump_reserved_desc(stage2, desc, level);
            }
            invalid = 0;
            break;
        case 3:
            indent(stage2, level);
            printk(fmt, i);
            if (level < 3) {
                dump_table_desc(stage2, desc, level);
            } else {
                dump_page_desc(stage2, desc, level);
            }
            invalid = 0;
            break;
        }
    }
}

void aarch64_stage2_dump_descriptor(struct aarch64_stage2 *stage2)
{
    uint64_t d;

    printk("<%s>\n", __func__);
    d = aarch64_stage2_vttbr_el2(stage2);
    printk("  VTTBR_EL2:%016llx\n", d);
    d = aarch64_stage2_vtcr_el2(stage2);
    printk("   VTCR_EL2:%016llx\n", d);
    printk("Start level: %u\n", stage2->base.start_level);
    if (stage2->base.granule == AARCH64_MMU_4KB_GRANULE) {
        aarch64_mmu_base_lock(&(stage2->base));
        dump_table(stage2, stage2->base.addr, stage2->base.start_level);
        aarch64_mmu_base_unlock(&(stage2->base));
    }
}

