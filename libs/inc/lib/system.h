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
#define ASSEMBLER
#endif

#ifndef ASSEMBLER

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* defines */

/* variables */

/* functions */

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLER */

#endif /* LIB_SYSTEM_H */

