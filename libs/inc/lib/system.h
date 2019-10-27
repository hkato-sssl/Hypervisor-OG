/*
 * lib/system.h
 *
 * (C) 2018 Hidekazu Kato
 */

#ifndef LIB_SYSTEM_H
#define LIB_SYSTEM_H

/* includes */

/* defines */

#ifdef	__ARMASM__	/* DS-5 assembler */
#define ASSEMBLY
#endif

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* defines */

/* variables */

/* functions */

void udelay(uint32_t usec);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* LIB_SYSTEM_H */

