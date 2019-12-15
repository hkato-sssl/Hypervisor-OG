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

void print_request(const struct vpc_memory_access_request *req)
{
    printk(" Access : %s\n", ((req->access == VPC_WRITE_ACCESS) ? "WRITE" : "READ"));
    printk("Address : %018p\n", req->addr);
    printk("   Size : %u\n", req->size);
    printk("   Sign : %u\n", req->flag.sign);
}

errno_t emulator_02(struct vpc *vpc, const struct vpc_memory_access_request *req)
{
    errno_t ret;

    printk("<%s>\n", __func__);
    print_request(req);

    ret = -ENOSYS;

    return ret;
}

