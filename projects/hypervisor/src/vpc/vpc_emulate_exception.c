/*
 * vpc/vpc_emulate_exception.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include "driver/aarch64/system_register/esr_elx.h"
#include "driver/aarch64/system_register/hcr_el2.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/thread.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static uint8_t esr_el2_ec(struct vpc *vpc)
{
    uint8_t ec;
    uint64_t esr;

    esr = vpc->regs[VPC_ESR_EL2];
    ec = (uint8_t)EXTRACT_ESR_EC(esr);

    return ec;
}

static errno_t call_syscall_emulator(struct insn *insn,
                                     vpc_syscall_emulator_t func)
{
    errno_t ret;

    if (func != NULL) {
        ret = (*func)(insn);
    } else {
        ret = -ENOTSUP;
    }

    return ret;
}

static errno_t call_exception_emulator(struct vpc *vpc,
                                       vpc_exception_emulator_t func)
{
    errno_t ret;

    if (func != NULL) {
        ret = (*func)(vpc);
    } else {
        ret = -ENOTSUP;
    }

    return ret;
}

static errno_t emulate_aarch64_synchronous(struct vpc *vpc)
{
    errno_t ret;
    uint8_t ec;
    struct insn insn;

    ec = esr_el2_ec(vpc);
    switch (ec) {
    case 0x15: /* 010101 - SVC instruction execution in AArch64 state */
        ret = insn_parse_aarch64_svc(&insn, vpc);
        if (ret == SUCCESS) {
            ret = call_syscall_emulator(&insn, vpc->exception.ops->aarch64.svc);
        }
        break;
    case 0x16: /* 010110 - HVC instruction execution in AArch64 state */
        ret = insn_parse_aarch64_hvc(&insn, vpc);
        if (ret == SUCCESS) {
            ret = call_syscall_emulator(&insn, vpc->exception.ops->aarch64.hvc);
        }
        break;
    case 0x17: /* 010111 - SMC instruction execution in AArch64 state */
        ret = insn_parse_aarch64_smc(&insn, vpc);
        if (ret == SUCCESS) {
            ret = call_syscall_emulator(&insn, vpc->exception.ops->aarch64.smc);
        }
        break;
    case 0x24: /*100100 - Data Abort from a lower Exception level */
        ret = call_exception_emulator(vpc,
                                      vpc->exception.ops->aarch64.data_abort);
        break;
    default:
        printk("%s: ESR_EL2.EC=0x%02x\n", __func__, ec);
        printk("%s: unknown value of EC.\n", __func__);
        ret = -ENOSYS;
    }

    return ret;
}

static errno_t emulate_irq(struct vpc *vpc)
{
    return call_exception_emulator(vpc, vpc->exception.ops->irq);
}

static errno_t emulate_fiq(struct vpc *vpc)
{
    return call_exception_emulator(vpc, vpc->exception.ops->fiq);
}

static errno_t emulate_serror(struct vpc *vpc)
{
    return -ENOSYS;
}

errno_t vpc_emulate_exception(struct vpc *vpc)
{
    errno_t ret;
    uint64_t d;

    d = thread_read_tls(TLS_EXCEPTION_VECTOR);
    switch (d) {
    case 0x0400: /* Synchronous */
        ret = emulate_aarch64_synchronous(vpc);
        break;
    case 0x0480: /* IRQ */
        ret = emulate_irq(vpc);
        break;
    case 0x0500: /* FIQ */
        ret = emulate_fiq(vpc);
        break;
    case 0x0580: /* SError */
        ret = emulate_serror(vpc);
        break;
    default:
        ret = -ENOSYS;
    }

    return ret;
}
