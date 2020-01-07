/*
 * driver/aarch64/at.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef DRIVER_AARCH64_AT_H
#define DRIVER_AARCH64_AT_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* defines */

/* types */

/* variables */

/* functions */

uint64_t aarch64_at_s1e0r(uint64_t va);
uint64_t aarch64_at_s1e0w(uint64_t va);
uint64_t aarch64_at_s1e1r(uint64_t va);
uint64_t aarch64_at_s1e1w(uint64_t va);

uint64_t aarch64_at_s12e0r(uint64_t va);
uint64_t aarch64_at_s12e0w(uint64_t va);
uint64_t aarch64_at_s12e1r(uint64_t va);
uint64_t aarch64_at_s12e1w(uint64_t va);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* DRIVER_AARCH64_AT_H */

