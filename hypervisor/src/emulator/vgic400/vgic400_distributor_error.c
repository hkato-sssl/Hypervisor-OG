/*
 * emulator/vgic400/vgic400_distributor_error.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "hypervisor/vpc.h"
#include "hypervisor/insn.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t vgic400_distributor_error(const struct insn *insn, const char *msg)
{
    printk("VGIC400-D: %s\n", msg);
    printk("           type = <%s>\n", (insn->type == INSN_TYPE_LDR) ? "LDR" : "STR");
    printk("           addr = %p\n", insn->op.ldr.ipa);
    printk("           size = %u\n", insn->op.ldr.size);

    return -EPERM;
}

