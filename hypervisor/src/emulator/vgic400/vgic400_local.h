/*
 * emulator/vgic400/vgic400_local.h
 *
 * (C) 2019 Hidekazu Kato
 */

#ifndef EMULATOR_VGIC400_VGIC400_LOCAL_H
#define EMULATOR_VGIC400_VGIC400_LOCAL_H

/* includes */

/* defines */

#ifndef ASSEMBLY

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

#include <stdint.h>
#include <stdbool.h>
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "lib/system/memio.h"
#include "hypervisor/vm.h"
#include "hypervisor/vpc.h"
#include "hypervisor/emulator/insn.h"
#include "hypervisor/emulator/vgic400.h"

/* defines */

#define ERR_MSG_UNAUTH  "An unauthorized access has occured."
#define ERR_MSG_OOR     "A register is out of range."

#define VGIC400_READ8(a)        REG_READ8((a), 0)
#define VGIC400_READ32(a)       REG_READ32((a), 0)
#define VGIC400_WRITE8(a, d)    REG_WRITE8((a), 0, (d))
#define VGIC400_WRITE32(a, d)   REG_WRITE32((a), 0, (d))

/* types */

/* variables */

/* inline functions */

static inline void gic400_write_virtif_control(struct vgic400 *vgic, uint32_t reg, uint32_t d)
{
    REG_WRITE32(vgic->reg_base, reg, d);
}

static inline uint32_t gic400_read_virtif_control(struct vgic400 *vgic, uint32_t reg)
{
    uint32_t d;

    d = REG_READ32(vgic->reg_base, reg);

    return d;
}

static inline bool is_target_irq(const struct vgic400 *vgic, uint16_t irq)
{
    uint32_t bit;

    bit = 1 << (irq & 31);

    return ((vgic->target.irq[irq / 32] & bit) != 0) ? true : false;
}

static inline bool is_aligned_word_access(const struct insn *insn)
{
    bool ret;

    if (IS_ALIGNED(insn->op.ldr.ipa, 4) && (insn->op.ldr.size == 4)) {
        ret = true;
    } else {
        ret = false;
    }

    return ret;
}

/* functions */

errno_t vgic400_distributor_ro_word_register(struct vgic400 *vgic, const struct insn *insn);
errno_t vgic400_distributor_bit_register(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg, uintptr_t base);
errno_t vgic400_distributor_byte_register(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg, uintptr_t base);
errno_t vgic400_distributor_ctlr(struct vgic400 *vgic, const struct insn *insn);
errno_t vgic400_distributor_typer(struct vgic400 *vgic, const struct insn *insn);
errno_t vgic400_distributor_spisr(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg);
errno_t vgic400_distributor_ipriorityr(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg);
errno_t vgic400_distributor_itargetsr(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg);
errno_t vgic400_distributor_igroupr(struct vgic400 *vgic, const struct insn *insn);
errno_t vgic400_distributor_sgir(struct vgic400 *vgic, const struct insn *insn);
errno_t vgic400_distributor_icfgr(struct vgic400 *vgic, const struct insn *insn, uintptr_t reg);

uint32_t vgic400_quad_byte_mask(const struct vgic400 *vgic, uint32_t irq);
uint64_t vgic400_p2v_cpu_map_b(uint64_t src, const struct vm *vm);
uint64_t vgic400_p2v_cpu_map_w(uint64_t src, const struct vm *vm);
uint64_t vgic400_v2p_cpu_map_b(uint64_t src, const struct vm *vm);
uint64_t vgic400_v2p_cpu_map_w(uint64_t src, const struct vm *vm);

errno_t vgic400_distributor_error(const struct insn *insn, const char *msg);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* EMULATOR_VGIC400_VGIC400_LOCAL_H */

