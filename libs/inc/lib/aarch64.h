/*
 * lib/aarch64.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef LIB_AARCH64_H
#define LIB_AARCH64_H

/* includes */

/* defines */

#ifdef ASSEMBLY
        .macro      BR_EL, table
        adr         ip0, \table
        mrs         ip1, CurrentEL
        add         ip0, ip0, ip1, lsl #1
        ldr         ip1, [ip0]
        br          ip1
        .balign      8
        .endm
#else /* ASSEMBLY */

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

#endif /* ASSEMBLY */

#endif /* LIB_AARCH64_H */

