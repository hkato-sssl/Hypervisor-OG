/*
 * aarch64/mmu/aarch64_mmu_init.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "lib/system/errno.h"
#include "driver/aarch64/mmu.h"
#include "mmu_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t validate_mmu_el(struct aarch64_mmu_trans_table_configuration const *config)
{
	errno_t ret;

	switch (config->el.level) {
	case 0:
	case 1:
		ret = SUCCESS;
		break;
	case 2:
		if (config->el.ns != 0) {
			ret = SUCCESS;
		} else {
			ret = -EINVAL;
		}
		break;
	case 3:
		if (config->el.ns == 0) {
			ret = SUCCESS;
		} else {
			ret = -EINVAL;
		}
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

static errno_t validate_parameters(struct aarch64_mmu_trans_table *tt, struct aarch64_mmu_trans_table_configuration const *config)
{
    errno_t ret;

    if ((tt != NULL) && (config != NULL) && (config->pool.block_sz == MMU_BLOCK_SZ)) {
        ret = validate_mmu_el(config);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t init_trans_table(struct aarch64_mmu_trans_table *tt, struct aarch64_mmu_trans_table_configuration const *config)
{
	tt->active = false;
    tt->asid = config->asid;
    memset(tt->addr, 0, MMU_BLOCK_SZ);

    tt->mair = config->mair;
	tt->el = config->el;
    tt->tcr = config->tcr;

    return SUCCESS;
}

static errno_t mmu_init(struct aarch64_mmu_trans_table *tt, struct aarch64_mmu_trans_table_configuration const *config)
{
    errno_t ret;

    memset(tt, 0, sizeof(*tt));

    ret = aarch64_mmu_block_pool_init(&(tt->pool), &(config->pool));
    if (ret == SUCCESS) {
        tt->addr = aarch64_mmu_block_calloc(&(tt->pool), MMU_BLOCK_SZ);
        if (tt->addr != NULL) {
            ret = init_trans_table(tt, config);
        } else {
            ret = -ENOMEM;
        }
    }

    return ret;
}

errno_t aarch64_mmu_init(struct aarch64_mmu_trans_table *tt, struct aarch64_mmu_trans_table_configuration const *config)
{
    errno_t ret;

    ret = validate_parameters(tt, config);
    if (ret == SUCCESS) {
        ret = mmu_init(tt, config);
    }

    return ret;
}

