/*
 * hypervisor/emulator/insn.h
 *
 * (C) 2020 Hidekazu Kato
 */

#ifndef HYPERVISOR_EMULATOR_INSN_H
#define HYPERVISOR_EMULATOR_INSN_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>
#include "lib/system/errno.h"

/* defines */

/* types */

struct vpc;

enum insn_type {
    INSN_TYPE_INVALID,
    INSN_TYPE_LDR,
    INSN_TYPE_STR
};

struct insn_ldr_str {
    uintptr_t       va;
    uintptr_t       ipa;
    uintptr_t       offset;
    uint8_t         size;

    struct {
        uint8_t     src;
        uint8_t     dst;
    } gpr;

    struct {
        uint8_t     sign:1;
        uint8_t     a32:1;
        uint8_t     wb:1;
        uint8_t     post:1;
    } flag;
};

struct insn {
    struct vpc      *vpc;
    enum insn_type  type;
    union {
        struct insn_ldr_str ldr;
        struct insn_ldr_str str;
    } op;
};

/* variables */

/* functions */

errno_t insn_generate_ldr_str(struct insn *insn, struct vpc *vpc);
uint64_t insn_str_src_value(const struct insn *insn);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_EMULATOR_INSN_H */

