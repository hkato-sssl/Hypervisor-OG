// SPDX-License-Identifier: Apache-2.0
/*
 * arm/smmu500/smmu500_dump.c
 *
 * (C) 2020 Hidekazu Kato
 */

#include "driver/arm/device/smmu500.h"
#include "driver/arm/device/smmuv2.h"
#include "driver/arm/smmu500.h"
#include "lib/system/memio.h"
#include "lib/system/printk.h"
#include "smmu500_local.h"
#include <stddef.h>
#include <stdint.h>

/* defines */

#define REGISTER(regs) \
    {                  \
#regs, regs    \
    }
#define REGISTER_ARRAY(regs) \
    {                        \
#regs, regs(0)       \
    }

/* types */

struct dump_register {
    const char *name;
    uint32_t regs;
};

/* prototypes */

/* variables */

static struct dump_register gr0[] = {
    REGISTER(SMMU_sCR0),
    REGISTER(SMMU_SCR1),
    REGISTER(SMMU_sCR2),
    REGISTER(SMMU_sACR),
    REGISTER(SMMU_IDR0),
    REGISTER(SMMU_IDR1),
    REGISTER(SMMU_IDR2),
    REGISTER(SMMU_IDR3),
    REGISTER(SMMU_IDR4),
    REGISTER(SMMU_IDR5),
    REGISTER(SMMU_IDR6),
    REGISTER(SMMU_IDR7),
    REGISTER(SMMU_sGFAR),
    REGISTER(SMMU_sGFSR),
    REGISTER(SMMU_sGFSRRESTORE),
    REGISTER(SMMU_sGFSYNR0),
    REGISTER(SMMU_sGFSYNR1),
    REGISTER(SMMU_sGFSYNR2),
    {NULL, 0} /* terminator */
};

static struct dump_register s1_cb[] = {
    REGISTER(SMMU_S1_CB_SCTLR),
    REGISTER(SMMU_S1_CB_ACTLR),
    /*  REGISTER(SMMU_S1_CB_RESUME),        Write-Only */
    REGISTER(SMMU_S1_CB_TCR2),
    REGISTER(SMMU_S1_CB_TTBR0),
    REGISTER(SMMU_S1_CB_TTBR1),
    REGISTER(SMMU_S1_CB_TCR),
    REGISTER(SMMU_S1_CB_CONTEXTIDR),
    REGISTER(SMMU_S1_CB_PRRR),
    REGISTER(SMMU_S1_CB_MAIR0),
    REGISTER(SMMU_S1_CB_NMRR),
    REGISTER(SMMU_S1_CB_MAIR1),
    REGISTER(SMMU_S1_CB_PAR),
    REGISTER(SMMU_S1_CB_FSR),
    /*  REGISTER(SMMU_S1_CB_FSRRESTORE),    Write-Only */
    REGISTER(SMMU_S1_CB_FAR),
    REGISTER(SMMU_S1_CB_FSYNR0),
    REGISTER(SMMU_S1_CB_FSYNR1),
    REGISTER(SMMU_S1_CB_TLBIVA),
    REGISTER(SMMU_S1_CB_TLBIVAA),
    REGISTER(SMMU_S1_CB_TLBIASID),
    REGISTER(SMMU_S1_CB_TLBIALL),
    REGISTER(SMMU_S1_CB_TLBIVAL),
    REGISTER(SMMU_S1_CB_TLBIVAAL),
    REGISTER(SMMU_S1_CB_TLBSYNC),
    REGISTER(SMMU_S1_CB_TLBSTATUS),
    REGISTER(SMMU_S1_CB_ATS1PR),
    REGISTER(SMMU_S1_CB_ATS1PW),
    REGISTER(SMMU_S1_CB_ATS1UR),
    REGISTER(SMMU_S1_CB_ATS1UW),
    REGISTER(SMMU_S1_CB_ATSR),
    {NULL, 0} /* terminator */
};

static struct dump_register s2_cb[] = {
    REGISTER(SMMU_S2_CB_SCTLR),
    REGISTER(SMMU_S2_CB_ACTLR),
    /*  REGISTER(SMMU_S2_CB_RESUME),        Write-Only */
    REGISTER(SMMU_S2_CB_TTBR0),
    REGISTER(SMMU_S2_CB_TCR),
    REGISTER(SMMU_S2_CB_FSR),
    /*  REGISTER(SMMU_S2_CB_FSRRESTORE),    Write-Only */
    REGISTER(SMMU_S2_CB_FAR),
    REGISTER(SMMU_S2_CB_FSYNR0),
    REGISTER(SMMU_S2_CB_FSYNR1),
    REGISTER(SMMU_S2_CB_IPAFAR),
    REGISTER(SMMU_S2_CB_TLBIIPAS2),
    REGISTER(SMMU_S2_CB_TLBIIPAS2L),
    REGISTER(SMMU_S2_CB_TLBSYNC),
    REGISTER(SMMU_S2_CB_TLBSTATUS),
    {NULL, 0} /* terminator */
};

/* functions */

static void dump_cb(const struct smmu500 *smmu, uint8_t n,
                    const struct dump_register *list)
{
    int i;

    for (i = 0; list[i].name != NULL; ++i) {
        printk("  %21s: 0x%08x\n", list[i].name,
               smmu500_cb_read32(smmu, n, list[i].regs));
    }
}

static void dump_s1_cb(const struct smmu500 *smmu, uint8_t n)
{
    dump_cb(smmu, n, s1_cb);
}

static void dump_s2_cb(const struct smmu500 *smmu, uint8_t n)
{
    dump_cb(smmu, n, s2_cb);
}

static void dump_stream_regs(const char *name, uint8_t n, uintptr_t base,
                             uint32_t regs)
{
    if (n < 10) {
        printk("%15s%u: 0x%08x\n", name, n, REG_READ32(base, regs));
    } else {
        printk("%14s%u: 0x%08x\n", name, n, REG_READ32(base, regs));
    }
}

static void dump_stream_match_register(const struct smmu500 *smmu, uint8_t id,
                                       int level)
{
    uint32_t cb;
    uint32_t s2cr;

    dump_stream_regs("SMMU_SMR", id, smmu->smmu_base, SMMU_SMR(id));
    dump_stream_regs("SMMU_S2CR", id, smmu->smmu_base, SMMU_S2CR(id));
    dump_stream_regs("SMMU_CBAR", id, smmu->smmu_gr1_base, SMMU_CBAR(id));
    dump_stream_regs("SMMU_CBA2R", id, smmu->smmu_gr1_base, SMMU_CBA2R(id));
    dump_stream_regs("SMMU_CBFRSYNRA", id, smmu->smmu_gr1_base,
                     SMMU_CBFRSYNRA(id));
    if (level > 0) {
        s2cr = smmu500_gr0_read32(smmu, SMMU_S2CR(id));
        cb = BF_EXTRACT(s2cr, 7, 0);
        switch (BF_EXTRACT(s2cr, 17, 16)) {
        case 0:
            dump_s2_cb(smmu, cb);
            break;
        case 1:
            dump_s1_cb(smmu, cb);
            break;
        default:
            /* no work */
            break;
        }
    }
}

static void dump_stream_match_registers(const struct smmu500 *smmu)
{
    uint8_t i;

    for (i = 0; i < smmu->nr_stream_matches; ++i) {
        dump_stream_match_register(smmu, i, 0);
    }
}

static void dump_gr0(const struct smmu500 *smmu)
{
    uint32_t i;

    printk("SMMU Global Register Space 0:\n");

    for (i = 0; gr0[i].name != NULL; ++i) {
        printk("%17s: 0x%08x\n", gr0[i].name,
               smmu500_gr0_read32(smmu, gr0[i].regs));
    }

    printk("\n");
}

static void dump_smmu500(const struct smmu500 *smmu)
{
    printk("<SMMU-500>\n");
    printk("                  smmu_base: %p\n", smmu->smmu_base);
    printk("              smmu_gr1_base: %p\n", smmu->smmu_gr1_base);
    printk("               smmu_cb_base: %p\n", smmu->smmu_cb_base);
    printk("                   nr_pages: %u\n", smmu->nr_pages);
    printk("                  page_size: %u\n", smmu->page_size);
    printk("                  vmid_size: %u\n", smmu->vmid_size);
    printk("          nr_stream_matches: %u\n", smmu->nr_stream_matches);
    printk("           nr_context_banks: %u\n", smmu->nr_context_banks);
    printk("        nr_s2_context_banks: %u\n", smmu->nr_s2_context_banks);
    printk("nr_context_fault_interrupts: %u\n",
           smmu->nr_context_fault_interrupts);
    printk("\n");
}

void smmu500_dump(struct smmu500 *smmu)
{
    smmu500_lock(smmu);

    dump_smmu500(smmu);
    dump_gr0(smmu);
    dump_stream_match_registers(smmu);

    smmu500_unlock(smmu);
}

void smmu500_dump_stream_match_register(struct smmu500 *smmu, uint8_t id)
{
    if (id < smmu->nr_stream_matches) {
        smmu500_lock(smmu);
        dump_stream_match_register(smmu, id, 1);
        smmu500_unlock(smmu);
    }
}
