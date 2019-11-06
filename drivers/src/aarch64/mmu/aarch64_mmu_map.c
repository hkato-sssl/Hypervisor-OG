/*
 * driver/aarch64/mmu/aarch64_mmu_map.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/aarch64/mmu.h"
#include "mmu_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t aarch64_mmu_map(struct aarch64_mmu_trans_table *tt, void *va, void *pa, size_t sz, struct aarch64_mmu_attr const *attr)
{
	errno_t ret;

	/* Support 4KB granule only */
	ret = aarch64_mmu_map_4KB_granule(tt, va, pa, sz, attr);

	return ret;
}
