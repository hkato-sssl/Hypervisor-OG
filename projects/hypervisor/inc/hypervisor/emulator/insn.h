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

enum insn_operand_type {
    INSN_TYPE_NULL,
    INSN_TYPE_LDR,
    INSN_TYPE_STR,
    INSN_TYPE_SVC,
    INSN_TYPE_HVC,
    INSN_TYPE_SMC,
};

struct insn_ldr_str {
    uint64_t        va;
    uint64_t        pa;
    uint64_t        offset;
    uint8_t         size;

    struct {
        uint8_t     src;
        uint8_t     dst;
    } gpr;

    struct {
        uint8_t     isv:1;      /* ESR_ELx.ISS.ISV */
        uint8_t     wreg:1;     /* 0: 64-bit, 1: 32-bit */
        uint8_t     sign:1;     /* Sign-extention */
        uint8_t     post:1;     /* Post-index */
        uint8_t     pre:1;      /* Pre-index */
    } flag;
};

struct insn_system_call {
    uint16_t        imm;
};

struct insn {
    struct vpc                  *vpc;
    enum insn_operand_type      type;
    union {
        struct insn_ldr_str     ldr;
        struct insn_ldr_str     str;
        struct insn_system_call system_call;
    } op;
};

/* variables */

/* functions */

errno_t insn_parse_aarch64_ldr_str(struct insn *insn, struct vpc *vpc);
errno_t insn_parse_aarch64_svc(struct insn *insn, struct vpc *vpc);
errno_t insn_parse_aarch64_hvc(struct insn *insn, struct vpc *vpc);
errno_t insn_parse_aarch64_smc(struct insn *insn, struct vpc *vpc);
uint64_t insn_str_src_value(const struct insn *insn);
errno_t insn_emulate_ldr(const struct insn *insn, uint64_t d);
errno_t insn_emulate_str(const struct insn *insn);
errno_t insn_emulate_indexed_addressing(const struct insn *insn);

/* for debugging */

void insn_print(const struct insn *insn);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* HYPERVISOR_EMULATOR_INSN_H */

