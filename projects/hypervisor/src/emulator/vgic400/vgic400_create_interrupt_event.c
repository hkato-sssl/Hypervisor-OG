// SPDX-License-Identifier: Apache-2.0
/*
 * emulator/vgic400/vgic400_create_interrupt_event.c
 *
 * (C) 2024 Hidekazu Kato
 */

#include "hypervisor/emulator/vgic400.h"
#include "hypervisor/vm.h"
#include "lib/system/errno.h"
#include "lib/system/printk.h"
#include "vgic400_local.h"
#include <stdint.h>

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t vgic400_create_interrupt_event(struct vpc *vpc, struct vgic400 *vgic,
                                       struct vgic400_interrupt_event *event,
                                       uint32_t iar)
{
    uint32_t irq;
    uint32_t virq;
    uint32_t cpuid;
    uint32_t vcpuid;

    event->iar = iar;

    irq = BF_EXTRACT(iar, 9, 0);
    virq = vgic400_irq_to_virq(vgic, irq);
    if (virq == VGIC400_NO_ASSIGNED) {
        printk("%s: InterruptID(=%u) is not assigned to the VM.\n", __func__,
               irq);
        return -EINVAL;
    }
    event->irq = virq;

    cpuid = BF_EXTRACT(iar, 12, 10);
    if (irq < 16) {
        vcpuid = vm_virtual_proc_no(vgic->vm, cpuid);
        if (vcpuid == VM_NO_ASSIGN) {
            printk("%s: CPUID(=%u) is not assigned to the VM.\n", __func__,
                   cpuid);
            return -EPERM;
        }
        event->cpuid = vcpuid;
        event->priority = vgic->sgi[vpc->proc_no].priority[irq];
    } else if (irq < 32) {
        event->cpuid = 0;
        event->priority = vgic->ppi[vpc->proc_no].priority[irq - 16];
    } else {
        event->cpuid = 0;
        event->priority = vgic->spi.priority[irq - 32];
    }

    return SUCCESS;
}
