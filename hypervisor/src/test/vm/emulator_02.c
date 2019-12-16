/*
 * test/vm/emulator_02.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/printk.h"
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

void print_access(const struct vpc_memory_access *access)
{
    printk(" Access : %s\n", ((access->request.type == VPC_WRITE_ACCESS) ? "WRITE" : "READ"));
    printk("Address : %018p\n", access->request.addr);
    printk("   Size : %u\n", access->request.size);
    printk("   Sign : %u\n", access->request.flag.sign);
}

errno_t emulator_02(struct vpc *vpc, const struct vpc_memory_access *access)
{
    errno_t ret;

    printk("<%s>\n", __func__);
    print_access(access);

    ret = -ENOSYS;

    return ret;
}

