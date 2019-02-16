/*
 * driver/aarch64/cache.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef DRIVER_AARCH64_CACHE_H
#define DRIVER_AARCH64_CACHE_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* defines */

/* types */

/* variables */

/* functions */

size_t aarch64_poc_line_sz(void);

void aarch64_icache_invd_all(void);
void aarch64_icache_invd(void *addr);
errno_t aarch64_icache_invd_range(void *st, void *ed);

void aarch64_dcache_invd_all(void);
void aarch64_dcache_invd(void *addr);
errno_t aarch64_dcache_invd_range(void *st, void *ed);

void aarch64_dcache_clean_all(void);
void aarch64_dcache_clean(void *addr);
errno_t aarch64_dcache_clean_range(void *st, void *ed);

void aarch64_dcache_clean_invd_all(void);
void aarch64_dcache_clean_invd(void *addr);
errno_t aarch64_dcache_clean_invd_range(void *st, void *ed);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_AARCH64_CACHE_H */

