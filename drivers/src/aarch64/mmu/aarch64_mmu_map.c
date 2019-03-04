/*
 * driver/aarch64/mmu/aarch64_mmu_map.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/aarch64/mmu.h"
/* defines */

#define	MEM_4KB		(4 * 1024)

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t mmu_map(struct aarch64_mmu_trans_table *tt, void **va, void **pa, size_t *sz, struct aarch64_mmu_attr const *attr)
{
	errno_t ret;

	if (*sz >= MEM_4KB) {
		ret = aarch64_mmu_map_4KB(tt, *va, *pa, attr);
		if (ret == SUCCESS) {
			*(uintptr_t **)va += MEM_4KB;
			*(uintptr_t **)pa += MEM_4KB;
			*sz -= MEM_4KB;
		}
	} else {
		ret = -EINVAL;
	}

	return ret;
}

errno_t aarch64_mmu_map(struct aarch64_mmu_trans_table *tt, void *va, void *pa, size_t sz, struct aarch64_mmu_attr const *attr)
{
	errno_t ret;

	do {
		ret = mmu_map(tt, &va, &pa, &sz, attr);
	} while ((ret == SUCCESS) && (sz != 0));

	return ret;
}
