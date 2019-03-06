/*
 * driver/aarch64/mmu/aarch64_mmu_map.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "driver/aarch64/mmu.h"
/* defines */

#define	MEM_4KB		(4UL * 1024)
#define	MEM_2MB		(2UL * 1024 * 1024)
#define	MEM_1GB		(1UL * 1024 * 1024 * 1024)

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t mmu_map(struct aarch64_mmu_trans_table *tt, void **va, void **pa, size_t *sz, struct aarch64_mmu_attr const *attr)
{
	errno_t ret;

	if ((*sz >= MEM_1GB) && ((ret = aarch64_mmu_map_1GB(tt, *va, *pa, attr)) == SUCCESS)) {
		*(char **)va += MEM_1GB;
		*(char **)pa += MEM_1GB;
		*sz -= MEM_1GB;
	} else if ((*sz >= MEM_2MB) && ((ret = aarch64_mmu_map_2MB(tt, *va, *pa, attr)) == SUCCESS)) {
		*(char **)va += MEM_2MB;
		*(char **)pa += MEM_2MB;
		*sz -= MEM_2MB;
	} else if ((*sz >= MEM_4KB) && ((ret = aarch64_mmu_map_4KB(tt, *va, *pa, attr)) == SUCCESS)) {
	    *(char **)va += MEM_4KB;
	    *(char **)pa += MEM_4KB;
	    *sz -= MEM_4KB;
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
