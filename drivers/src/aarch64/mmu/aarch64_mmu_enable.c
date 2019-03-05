/*
 * driver/aarch64/mmu/aarch64_mmu_enable.c
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

errno_t aarch64_mmu_enable(struct aarch64_mmu_trans_table const *tt)
{
	errno_t ret;

	ret = aarch64_mmu_enable_asm(tt);

	return ret;
}
