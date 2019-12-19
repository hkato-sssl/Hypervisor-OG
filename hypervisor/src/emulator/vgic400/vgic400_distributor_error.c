/*
 * emulator/vgic400/vgic400_distributor_error.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/printk.h"
#include "hypervisor/vpc.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

void vgic400_distributor_error(const struct vpc_memory_access *access, const char *msg)
{
    printk("VGIC400-D: %s\n", msg);
    printk("           type = <%s>\n", (access->request.type == VPC_READ_ACCESS) ? "READ" : "WRITE");
    printk("           addr = %p\n", access->request.addr);
    printk("           size = %u\n", access->request.size);
}

