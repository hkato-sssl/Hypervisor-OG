/*
 * emulate/emulate_exception.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "lib/bit.h"
#include "hypervisor/tls.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"
#include "hypervisor/emulate.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t call_emulator(struct vpc *vpc, emulate_func_t func)
{
    errno_t ret;

    if (func != NULL) {
        ret = (*func)(vpc);
    } else {
        ret = -ENOSYS;
    }

    return ret;
}

static errno_t emulate_synchronous(struct vpc *vpc)
{
    errno_t ret;
    uint64_t d;
    uint32_t ec;

    d = vpc->regs[VPC_ESR_EL2];
    ec = (uint32_t)BF_EXTRACT(d, 31, 26);
    switch (ec) {
    case 0x15:  /* 010101 - SVC instruction execution in AArch64 state */
        ret = call_emulator(vpc, vpc->emulate.ops->aarch64.svc);
        break;
    case 0x16:  /* 010110 - HVC instruction execution in AArch64 state */
        ret = call_emulator(vpc, vpc->emulate.ops->aarch64.hvc);
        break;
    case 0x17:  /* 010111 - SMC instruction execution in AArch64 state */
        ret = call_emulator(vpc, vpc->emulate.ops->aarch64.smc);
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
    return call_emulator(vpc, vpc->emulate.ops->irq);
}

static errno_t emulate_fiq(struct vpc *vpc)
{
    return call_emulator(vpc, vpc->emulate.ops->fiq);
}

static errno_t emulate_serror(struct vpc *vpc)
{
    return -ENOSYS;
}

errno_t emulate_exception(struct vpc *vpc)
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

