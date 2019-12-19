/*
 * emulator/vgic400/vgic400_distributor_emulate_memory_access.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stddef.h>
#include <stdint.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "lib/system/printk.h"
#include "driver/arm/gic400.h"
#include "driver/arm/device/gic400.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

#define IN_RANGE(name, reg) ((name(0) <= reg) && (reg < name(NR_GIC400_INTERRUPTS)))

/* types */

/* prototypes */

/* variables */

/* functions */

errno_t vgic400_distributor_emulate_memory_access(struct vgic400 *vgic, const struct vpc_memory_access *access)
{
    errno_t ret;
    uintptr_t base;
    uintptr_t reg;

    base = (uintptr_t)gic400_distributor_register_base(vgic->gic);
    reg = (access->request.addr - base);
    if (reg == GICD_CTLR) {
        ret = vgic400_distributor_ro_word_register(vgic, access, reg);
    } else if ((reg == GICD_TYPER)) {
        ret = vgic400_distributor_typer(vgic, access);
    } else if (reg == GICD_IIDR) {
        ret = vgic400_distributor_ro_word_register(vgic, access, reg);
    } else if (IN_RANGE(GICD_IGROUPR, reg)) {
        ret = vgic400_distributor_bit_register(vgic, access, reg, GICD_IGROUPR(0));
    } else if (IN_RANGE(GICD_ISENABLER, reg)) {
        ret = vgic400_distributor_bit_register(vgic, access, reg, GICD_ISENABLER(0));
    } else if (IN_RANGE(GICD_ICENABLER, reg)) {
        ret = vgic400_distributor_bit_register(vgic, access, reg, GICD_ICENABLER(0));
    } else if (IN_RANGE(GICD_ISPENDR, reg)) {
        ret = vgic400_distributor_bit_register(vgic, access, reg, GICD_ISPENDR(0));
    } else if (IN_RANGE(GICD_ICPENDR, reg)) {
        ret = vgic400_distributor_bit_register(vgic, access, reg, GICD_ICPENDR(0));
    } else if (IN_RANGE(GICD_ISACTIVER, reg)) {
        ret = vgic400_distributor_bit_register(vgic, access, reg, GICD_ISACTIVER(0));
    } else if (IN_RANGE(GICD_ICACTIVER, reg)) {
        ret = vgic400_distributor_bit_register(vgic, access, reg, GICD_ICACTIVER(0));
    } else if (IN_RANGE(GICD_IPRIORITYR, reg)) {
        ret = vgic400_distributor_byte_register(vgic, access, reg, GICD_IPRIORITYR(0));
    } else if (IN_RANGE(GICD_ITARGETSR, reg)) {
        ret = vgic400_distributor_itargetsr(vgic, access, reg);
    } else if (IN_RANGE(GICD_ICFGR, reg)) {
        ret = vgic400_distributor_icfgr(vgic, access, reg);
    } else if (reg == GICD_PPISR) {
        ret = vgic400_distributor_ro_word_register(vgic, access, reg);
    } else if (IN_RANGE(GICD_SPISR, reg)) {
        ret = vgic400_distributor_spisr(vgic, access, reg);
    } else if (reg == GICD_SGIR) {
        ret = vgic400_distributor_sgir(vgic, access);
    } else if (IN_RANGE(GICD_CPENDSGIR, reg)) {
        ret = vgic400_distributor_byte_register(vgic, access, reg, GICD_CPENDSGIR(0));
    } else if (IN_RANGE(GICD_SPENDSGIR, reg)) {
        ret = vgic400_distributor_byte_register(vgic, access, reg, GICD_SPENDSGIR(0));
    } else if ((reg == GICD_PIDR4) || (reg == GICD_PIDR5) ||
               (reg == GICD_PIDR6) || (reg == GICD_PIDR7) ||
               (reg == GICD_PIDR0) || (reg == GICD_PIDR1) ||
               (reg == GICD_PIDR2) || (reg == GICD_PIDR3) ||
               (reg == GICD_CIDR0) || (reg == GICD_CIDR1) ||
               (reg == GICD_CIDR2) || (reg == GICD_CIDR3)) {
        ret = vgic400_distributor_ro_word_register(vgic, access, reg);
    } else {
        vgic400_distributor_error(access, ERR_MSG_OOR);
        ret = -EIO;
    }

    return ret;
}

