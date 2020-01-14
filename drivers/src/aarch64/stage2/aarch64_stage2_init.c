/*
 * aarch64/stage2/aarch64_stage2_init.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "driver/aarch64/system_register.h"
#include "driver/aarch64/system_register/vtcr_el2.h"
#include "driver/aarch64/cache.h"
#include "driver/aarch64/stage2.h"
#include "stage2_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

static const struct aarch64_mmu_ops stage2_ops = {
    (aarch64_mmu_desc_func_t)aarch64_stage2_table_descriptor,
    (aarch64_mmu_desc_func_t)aarch64_stage2_block_descriptor,
    (aarch64_mmu_desc_func_t)aarch64_stage2_page_descriptor
};

/* functions */

static uint8_t vtcr_el2_sl0(const struct aarch64_stage2_configuration *config)
{
    uint8_t sl0;

    if (config->base.granule == AARCH64_MMU_4KB_GRANULE) {
        sl0 = 2 - config->start_level;
    } else {
        sl0 = 3 - config->start_level;
    }

    return sl0;
}

static uint8_t pa_width(const struct aarch64_stage2_configuration *config)
{
    static const uint8_t width[6] = { 32, 36, 40, 42, 44, 48 };

    return width[config->pa_range];
}

static uint64_t generate_vtcr_el2(const struct aarch64_stage2_configuration *config)
{
    uint64_t d;

    d = VTCR_EL2_RES1;
    d |= VTCR_EL2_PS(config->pa_range);
    d |= VTCR_EL2_SH0(config->sh);
    d |= VTCR_EL2_ORGN0(config->orgn);
    d |= VTCR_EL2_IRGN0(config->irgn);
    d |= VTCR_EL2_SL0(vtcr_el2_sl0(config));
    d |= VTCR_EL2_T0SZ(64 - pa_width(config));

    if (config->base.granule == AARCH64_MMU_64KB_GRANULE) {
        d |= VTCR_EL2_TG0(1);
    } else if (config->base.granule == AARCH64_MMU_16KB_GRANULE) {
        d |= VTCR_EL2_TG0(2);
    }

    return d;
}

static bool is_valid_pa_range(const struct aarch64_stage2_configuration *config)
{
    bool valid;
    uint64_t pa_range;

    pa_range = aarch64_read_id_aa64mmfr0_el1() & BITS(3, 0);
    valid = (config->pa_range <= pa_range) ? true : false;

    return valid;
}

static bool is_valid_start_level(const struct aarch64_stage2_configuration *config)
{
    bool valid;

    switch (config->base.granule) {
    case AARCH64_MMU_4KB_GRANULE:
        if (config->first_table == NULL) {
            if (config->start_level == 0) {
                valid = true;
            } else {
                valid = false;
            }
        } else if (IS_ALIGNED(config->first_table, MEM_4KB)) {
            if (config->start_level < 3) {
                valid = true;
            } else {
                valid = false;
            }
        } else {
            valid = false;
        }
        break;
    default:
        valid = false;
    }

    return valid;
}

static errno_t validate_parameters(struct aarch64_stage2 *stage2, const struct aarch64_stage2_configuration *config)
{
    errno_t ret;

    if ((stage2 != NULL) && (config != NULL) && (config->base.pool != NULL) && (config->base.pool->block_sz == MMU_BLOCK_SZ)) {
        if ((config->base.type == AARCH64_MMU_STAGE2) && is_valid_pa_range(config) && is_valid_start_level(config)) {
            ret = SUCCESS;
        } else {
            ret = -EINVAL;
        }
    } else {
        ret = -EINVAL;
    }

    return ret;
}

static errno_t stage2_init(struct aarch64_stage2 *stage2, const struct aarch64_stage2_configuration *config)
{
    errno_t ret;

    memset(stage2, 0, sizeof(*stage2));

    stage2->base.active = false;
    stage2->base.type = config->base.type;
    stage2->base.granule = config->base.granule;
    stage2->base.ops = &stage2_ops;
    stage2->base.pool = config->base.pool;
    stage2->base.start_level = config->start_level;
    stage2->vmid = config->vmid;
    stage2->pa_width = pa_width(config);
    stage2->pa_mask = BITS(63, stage2->pa_width);
    stage2->vtcr_el2 = generate_vtcr_el2(config);



    if (config->first_table != NULL) {
        stage2->base.addr = config->first_table;
        ret = SUCCESS;
    } else {
        stage2->base.addr = aarch64_mmu_block_calloc(stage2->base.pool, MMU_BLOCK_SZ);
        if (stage2->base.addr != NULL) {
            ret = SUCCESS;
        } else {
            ret = -ENOMEM;
        }
    }

    return ret;
}

errno_t aarch64_stage2_init(struct aarch64_stage2 *stage2, const struct aarch64_stage2_configuration *config)
{
    errno_t ret;

    ret = validate_parameters(stage2, config);
    if (ret == SUCCESS) {
        ret = stage2_init(stage2, config);
    }

    return ret;
}

