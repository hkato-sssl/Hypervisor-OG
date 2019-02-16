/*
 * aarch64/mmu/aarch64_mmu_init.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "lib/system/errno.h"
#include "driver/aarch64/mmu.h"
#include "mmu_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static bool is_valid_parameters(struct aarch64_mmu_trans_table *tt, struct aarch64_mmu_trans_table_configure const *conf)
{
    bool valid;

    if ((tt != NULL) && (conf != NULL) && (conf->pool.block_sz == MMU_BLOCK_SZ)) {
        valid = true;
    } else {
        valid = false;
    }

    return valid;
}

static errno_t init_trans_table(struct aarch64_mmu_trans_table *tt, struct aarch64_mmu_trans_table_configure const *conf)
{
    tt->asid = conf->asid;
    aarch64_mmu_memclr(tt->addr, MMU_BLOCK_SZ);

    /*
     * 現状ではTCR関連のパラメータは固定値とする
     */
    tt->tcr.sz = 16;    /* The region size is 2^48 bytes. */
    tt->tcr.sh = MMU_TCR_SH_ISH;
    tt->tcr.irgn = MMU_TCR_RGN_WBWA;
    tt->tcr.orgn = MMU_TCR_RGN_WBWA;

    return SUCCESS;
}

static errno_t mmu_init(struct aarch64_mmu_trans_table *tt, struct aarch64_mmu_trans_table_configure const *conf)
{
    errno_t ret;

    memset(tt, 0, sizeof(*tt));

    ret = aarch64_mmu_block_pool_init(&(tt->pool), &(conf->pool));
    if (ret == SUCCESS) {
        tt->addr = aarch64_mmu_block_alloc(&(tt->pool), MMU_BLOCK_SZ);
        if (tt->addr != NULL) {
            ret = init_trans_table(tt, conf);
        } else {
            ret = -ENOMEM;
        }
    }

    return ret;
}

errno_t aarch64_mmu_init(struct aarch64_mmu_trans_table *tt, struct aarch64_mmu_trans_table_configure const *conf)
{
    errno_t ret;

    if (is_valid_parameters(tt, conf)) {
        ret = mmu_init(tt, conf);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

