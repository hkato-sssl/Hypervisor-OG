/*
 * lib/aarch64.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef LIB_AARCH64_H
#define LIB_AARCH64_H

/* includes */

#include "lib/bit.h"

/* defines */

/* DAIF */

#define	DAIF_D		BIT(3)
#define	DAIF_A		BIT(2)
#define	DAIF_I		BIT(1)
#define	DAIF_F		BIT(0)
#define	DAIF_ALL	BITS(3, 0)

#ifndef ASSEMBLER

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* defines */

/* types */

/* variables */

/* functions */

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLER */

#endif /* LIB_AARCH64_H */

