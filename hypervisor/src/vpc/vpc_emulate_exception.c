/*
 * vpc/vpc_emulate_exception.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "lib/bit.h"
#include "driver/aarch64/system_register/hcr_el2.h"
#include "hypervisor/tls.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"
#include "hypervisor/emulator.h"

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
    ec = (uint8_t)BF_EXTRACT(esr, 31, 26);

    return ec;
}

static bool is_aarch64_state(const struct vpc *vpc)
{
    bool ret;

    if ((vpc->regs[VPC_HCR_EL2] & HCR_EL2_RW) == 0) {
        ret = false;
    } else if ((vpc->regs[VPC_SPSR_EL2] & BIT(4)) != 0) {  /* PSTATE.nRW */
        ret = false;
    } else {
        ret = true;
    }

    return ret;
}

static errno_t call_emulator(struct vpc *vpc, emulator_func_t func)
{
    errno_t ret;

    if (func != NULL) {
        ret = (*func)(vpc);
    } else {
        ret = -ENOSYS;
    }

    return ret;
}

static errno_t emulate_aarch64_synchronous(struct vpc *vpc)
{
    errno_t ret;
    uint8_t ec;

    ec = esr_el2_ec(vpc);
    switch (ec) {
    case 0x15:  /* 010101 - SVC instruction execution in AArch64 state */
        ret = call_emulator(vpc, vpc->emulator.ops->aarch64.svc);
        break;
    case 0x16:  /* 010110 - HVC instruction execution in AArch64 state */
        ret = call_emulator(vpc, vpc->emulator.ops->aarch64.hvc);
        break;
    case 0x17:  /* 010111 - SMC instruction execution in AArch64 state */
        ret = call_emulator(vpc, vpc->emulator.ops->aarch64.smc);
        break;
    case 0x24:  /*100100 - Data Abort from a lower Exception level */
        ret = call_emulator(vpc, vpc->emulator.ops->aarch64.data_abort);
        break;
    default:
        printk("%s: ESR_EL2.EC=0x%02x\n", __func__, ec);
        printk("%s: unknown value of EC.\n", __func__);
        ret = -ENOSYS;
    }

    return ret;
}

static errno_t emulate_synchronous(struct vpc *vpc)
{
    errno_t ret;

    if (is_aarch64_state(vpc)) {
        ret = emulate_aarch64_synchronous(vpc);
    } else {
        ret = -ENOSYS;
    }

    return ret;
}

static errno_t emulate_irq(struct vpc *vpc)
{
    return call_emulator(vpc, vpc->emulator.ops->irq);
}

static errno_t emulate_fiq(struct vpc *vpc)
{
    return call_emulator(vpc, vpc->emulator.ops->fiq);
}

static errno_t emulate_serror(struct vpc *vpc)
{
    return -ENOSYS;
}

errno_t vpc_emulate_exception(struct vpc *vpc)
{
    errno_t ret;
    uint64_t d;

    d = tls_read(TLS_EXCEPTION_VECTOR);
    printk("%s: vector=0x%04x\n", __func__, d);
    switch (d) {
    case 0x0400:    /* Synchronous */
        ret = emulate_synchronous(vpc);
        break;
    case 0x0480:    /* IRQ */
        ret = emulate_irq(vpc);
        break;
    case 0x0500:    /* FIQ */
        ret = emulate_fiq(vpc);
        break;
    case 0x0580:    /* SError */
        ret = emulate_serror(vpc);
        break;
    default:
        ret = -ENOSYS;
    }

    return ret;
}

