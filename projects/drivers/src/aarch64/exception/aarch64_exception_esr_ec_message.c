// SPDX-License-Identifier: Apache-2.0
/*
 * aarch64/exception/aarch64_exception_esr_ec_message.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/aarch64/exception.h"
#include <stdint.h>

/* defines */

/* types */

/* variables */

/* functions */

const char *aarch64_exception_esr_ec_message(uint32_t ec)
{
    const char *msg;

    switch (ec) {
    case 0x00: /* 0b000000 */
        msg = "Unknown reason";
        break;

    case 0x01: /* 0b000001 */
        msg = "Trapped WFI or WFE instruction execution";
        break;

    case 0x03: /* 0b000011 */
        msg = "Trapped MCR or MRC access with (coproc==0b1111) that is not "
              "reported using EC 0b000000";
        break;

    case 0x04: /* 0b000100 */
        msg = "Trapped MCRR or MRRC access with (coproc==0b1111) that is not "
              "reported using EC 0b000000";
        break;

    case 0x05: /* 0b000101 */
        msg = "Trapped MCR or MRC access with (coproc==0b1110)";
        break;

    case 0x06: /* 0b000110 */
        msg = "Trapped LDC or STC access";
        break;

    case 0x07: /* 0b000111 */
        msg = "Access to SVE, Advanced SIMD or floating-point functionality "
              "trapped by CPACR_EL1.FPEN or CPTR_ELx.TFP controle";
        break;

    case 0x08: /* 0b001000 */
        msg = "Trapped VMRS access, from ID group traps, that is not reported "
              "using EC 0b000111f";
        break;

    case 0x09: /* 0b001001 */
        msg = "Trapped access to an ARMv8.3-PAuth instruction";
        break;

    case 0x0c: /* 0b001100 */
        msg = "Trapped MRRC access with (coproc==0b1110)";
        break;

    case 0x0e: /* 0b001110 */
        msg = "Illegal Execution state";
        break;

    case 0x11: /* 0b010001 */
        msg = "SVC instruction execution in AArch32 state";
        break;

    case 0x12: /* 0b010010 */
        msg = "HVC instruction execution in AArch32 state, when HVC is not "
              "disabled";
        break;

    case 0x13: /* 0b010011 */
        msg = "SMC instruction execution in AArch32 state, when SMC is not "
              "disabled";
        break;

    case 0x15: /* 0b010101 */
        msg = "SVC instruction execution in AArch64 state";
        break;

    case 0x16: /* 0b010110 */
        msg = "HVC instruction execution in AArch64 state, when HVC is not "
              "disabled";
        break;

    case 0x17: /* 0b010111 */
        msg = "SMC instruction execution in AArch64 state, when SMC is not "
              "disabled";
        break;

    case 0x18: /* 0b011000 */
        msg = "Trapped MSR, MRS, or System instruction execution, that is not "
              "reported using EC 0x00, 0x01, or 0x07 When ARMv8.4-IDST is "
              "implemented, trapped ID registers";
        break;

    case 0x19: /* 0b011001 */
        msg = "Trapped access to SVE functionality, that is not reported using "
              "EC 0b000000";
        break;

    case 0x1a: /* 0b011010 */
        msg = "Trapped ERET, ERETAA or ERETAB instruction execution";
        break;

    case 0x1f: /* 0b011111 */
        msg = "IMPLEMENTATION DEFINED exception taken to EL3";
        break;

    case 0x20: /* 0b100000 */
        msg = "Instruction Abort from a lower Exception level";
        break;

    case 0x21: /* 0b100001 */
        msg = "Instruction Abort taken without a change in Exception level";
        break;

    case 0x22: /* 0b100010 */
        msg = "PC alignment fault";
        break;

    case 0x24: /* 0b100100 */
        msg = "Data Abort from a lower Exception level";
        break;

    case 0x25: /* 0b100101 */
        msg = "Data Abort taken without a change in Exception level";
        break;

    case 0x26: /* 0b100110 */
        msg = "SP alignment fault";
        break;

    case 0x28: /* 0b101000 */
        msg = "Trapped floating-point exception taken from AArch32 state";
        break;

    case 0x2c: /* 0b101100 */
        msg = "Trapped floating-point exception taken from AArch64 state";
        break;

    case 0x2f: /* 0b101111 */
        msg = "SError interrupt";
        break;

    case 0x30: /* 0b110000 */
        msg = "Breakpoint exception from a lower Exception level";
        break;

    case 0x31: /* 0b110001 */
        msg = "Breakpoint exception taken without a change in Exception level";
        break;

    case 0x32: /* 0b110010 */
        msg = "Software Step exception from a lower Exception level";
        break;

    case 0x33: /* 0b110011 */
        msg =
            "Software Step exception taken without a change in Exception level";
        break;

    case 0x34: /* 0b110100 */
        msg = "Watchpoint exception from a lower Exception level";
        break;

    case 0x35: /* 0b110101 */
        msg = "Watchpoint exception taken without a change in Exception level";
        break;

    case 0x38: /* 0b111000 */
        msg = "BKPT instruction execution in AArch32 state";
        break;

    case 0x3a: /* 0b111010 */
        msg = "Vector Catch exception from AArch32 state";
        break;

    case 0x3c: /* 0b111100 */
        msg = "BRK instruction execution in AArch64 state";
        break;

    default:
        msg = "**** Undefined Exception Class ****";
        break;
    }

    return msg;
}
