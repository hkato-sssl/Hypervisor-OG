/*
 * emulator/vgic400/vgic400_distributor_emulate_memory_insn.c
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
#include "hypervisor/emulator/insn.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static errno_t emulate_memory_insn(const struct insn *insn, struct vgic400 *vgic)
{
    errno_t ret;
    uintptr_t base;
    uintptr_t reg;

printk("%s: addr = %p\n", __func__, insn->op.ldr.ipa);
printk("%s: size = %u\n", __func__, insn->op.ldr.size);

    base = (uintptr_t)gic400_distributor_register_base(vgic->gic);
    reg = (insn->op.ldr.ipa - base);
    if (reg == 0x0000) {    /* GICD_CTLR */
        ret = vgic400_distributor_ctlr(vgic, insn);
    } else if (reg == 0x0004) { /* GICD_TYPER */
        ret = vgic400_distributor_typer(vgic, insn);
    } else if (reg == 0x0008) { /* GICD_IIDR */
        ret = vgic400_distributor_ro_word_register(vgic, insn);
    } else if ((0x0080 <= reg) && (reg <= 0x00bf)) {    /* GICD_IGROUPRn */
        ret = vgic400_distributor_igroupr(vgic, insn);
    } else if ((0x0100 <= reg) && (reg <= 0x013f)) {    /* GICD_ISENABLERn */
        ret = vgic400_distributor_bit_register(vgic, insn, reg, GICD_ISENABLER(0));
    } else if ((0x0180 <= reg) && (reg <= 0x01bf)) {    /* GICD_ICENABLERn */
        ret = vgic400_distributor_bit_register(vgic, insn, reg, GICD_ICENABLER(0));
    } else if ((0x0200 <= reg) && (reg <= 0x023f)) {    /* GICD_ISPENDRn */
        ret = vgic400_distributor_bit_register(vgic, insn, reg, GICD_ISPENDR(0));
    } else if ((0x0280 <= reg) && (reg <= 0x02bf)) {    /* GICD_ICPENDRn */
        ret = vgic400_distributor_bit_register(vgic, insn, reg, GICD_ICPENDR(0));
    } else if ((0x0300 <= reg) && (reg <= 0x033f)) {    /* GICD_ISACTIVERn */
        ret = vgic400_distributor_bit_register(vgic, insn, reg, GICD_ISACTIVER(0));
    } else if ((0x0380 <= reg) && (reg <= 0x03bf)) {    /* GICD_ICACTIVERn */
        ret = vgic400_distributor_bit_register(vgic, insn, reg, GICD_ICACTIVER(0));
    } else if ((0x0400 <= reg) && (reg <= 0x05ff)) {    /* GICD_IPRIORITYRn */
        ret = vgic400_distributor_byte_register(vgic, insn, reg, GICD_IPRIORITYR(0));
    } else if ((0x0800 <= reg) && (reg <= 0x09ff)) {    /* GICD_ITARGETSRn */
        ret = vgic400_distributor_itargetsr(vgic, insn, reg);
    } else if ((0x0c00 <= reg) && (reg <= 0x0c7f)) {    /* GICD_ICFGRn */
        ret = vgic400_distributor_icfgr(vgic, insn, reg);
    } else if (reg == 0x0d00) { /* GICD_PPISR */
        ret = vgic400_distributor_ro_word_register(vgic, insn);
    } else if ((0x0d04 <= reg) && (reg <= 0x0d3f)) {    /* GICD_SPISRn */
        ret = vgic400_distributor_spisr(vgic, insn, reg);
    } else if (reg == 0x0f00) { /* GICD_SGIR */
        ret = vgic400_distributor_sgir(vgic, insn);
    } else if ((0x0f10 <= reg) && (reg <= 0x0f1f)) {    /* GICD_CPENDSGIRn */
        ret = vgic400_distributor_byte_register(vgic, insn, reg, GICD_CPENDSGIR(0));
    } else if ((0x0f20 <= reg) && (reg <= 0x0f2f)) {    /* GICD_SPENDSGIRn */
        ret = vgic400_distributor_byte_register(vgic, insn, reg, GICD_SPENDSGIR(0));
    } else if (reg == 0x0fd0) { /* GICD_PIDR4 */
        ret = vgic400_distributor_ro_word_register(vgic, insn);
    } else if (reg == 0x0fd4) { /* GICD_PIDR5 */
        ret = vgic400_distributor_ro_word_register(vgic, insn);
    } else if (reg == 0x0fd8) { /* GICD_PIDR6 */
        ret = vgic400_distributor_ro_word_register(vgic, insn);
    } else if (reg == 0x0fdc) { /* GICD_PIDR7 */
        ret = vgic400_distributor_ro_word_register(vgic, insn);
    } else if (reg == 0x0fe0) { /* GICD_PIDR0 */
        ret = vgic400_distributor_ro_word_register(vgic, insn);
    } else if (reg == 0x0fe4) { /* GICD_PIDR1 */
        ret = vgic400_distributor_ro_word_register(vgic, insn);
    } else if (reg == 0x0fe8) { /* GICD_PIDR2 */
        ret = vgic400_distributor_ro_word_register(vgic, insn);
    } else if (reg == 0x0fec) { /* GICD_PIDR3 */
        ret = vgic400_distributor_ro_word_register(vgic, insn);
    } else if (reg == 0x0ff0) { /* GICD_CIDR0 */
        ret = vgic400_distributor_ro_word_register(vgic, insn);
    } else if (reg == 0x0ff4) { /* GICD_CIDR1 */
        ret = vgic400_distributor_ro_word_register(vgic, insn);
    } else if (reg == 0x0ff8) { /* GICD_CIDR2 */
        ret = vgic400_distributor_ro_word_register(vgic, insn);
    } else if (reg == 0x0ffc) { /* GICD_CIDR3 */
        ret = vgic400_distributor_ro_word_register(vgic, insn);
    } else {
        ret = vgic400_distributor_error(insn, ERR_MSG_OOR);
    }

    if (ret == SUCCESS) {
        ret = vpc_update_pc(insn->vpc);
    }

    return ret;
}

errno_t vgic400_distributor_emulate_memory_insn(const struct insn *insn, struct vgic400 *vgic)
{
    errno_t ret;

    if (((insn->type == INSN_TYPE_LDR) || (insn->type == INSN_TYPE_STR)) && (vgic != NULL)) {
        ret = emulate_memory_insn(insn, vgic);
    } else {
        ret = -EINVAL;
    }

    return ret;
}

