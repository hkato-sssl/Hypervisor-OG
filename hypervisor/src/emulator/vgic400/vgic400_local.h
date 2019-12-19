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
#include "hypervisor/vpc.h"
#include "hypervisor/vm.h"
#include "hypervisor/emulator/vgic400.h"

/* defines */

#define ERR_MSG_UNAUTH  "An unauthorized access has occured."
#define ERR_MSG_OOR     "A register is out of range."

#define VGIC400_READ8(a)        (*(volatile uint8_t *)(a))
#define VGIC400_READ32(a)       (*(volatile uint32_t *)(a))
#define VGIC400_WRITE8(a, d)    (*(volatile uint8_t *)(a) = (uint8_t)(d))
#define VGIC400_WRITE32(a, d)   (*(volatile uint32_t *)(a) = (uint32_t)(d))

/* types */

/* variables */

/* inline functions */

static inline bool is_active_irq(const struct vgic400 *vgic, uint16_t irq)
{
    uint32_t bit;

    bit = 1 << (irq & 31);

    return ((vgic->active.irq[irq / 32] & bit) != 0) ? true : false;
}

static inline uint64_t gpr_value(const struct vpc_memory_access *access)
{
    return access->vpc->regs[access->request.gpr];
}

static inline bool is_aligned_word_access(const struct vpc_memory_access *access)
{
    bool ret;

    if (IS_ALIGNED(access->request.addr, 4) && (access->request.size == 4)) {
        ret = true;
    } else {
        ret = false;
    }

    return ret;
}

/* functions */

errno_t vgic400_distributor_ro_word_register(struct vgic400 *vgic, const struct vpc_memory_access *access, uintptr_t reg);
errno_t vgic400_distributor_bit_register(struct vgic400 *vgic, const struct vpc_memory_access *access, uintptr_t reg, uintptr_t base);
errno_t vgic400_distributor_byte_register(struct vgic400 *vgic, const struct vpc_memory_access *access, uintptr_t reg, uintptr_t base);
errno_t vgic400_distributor_typer(struct vgic400 *vgic, const struct vpc_memory_access *access);
errno_t vgic400_distributor_spisr(struct vgic400 *vgic, const struct vpc_memory_access *access, uintptr_t reg);
errno_t vgic400_distributor_itargetsr(struct vgic400 *vgic, const struct vpc_memory_access *access, uintptr_t reg);
errno_t vgic400_distributor_sgir(struct vgic400 *vgic, const struct vpc_memory_access *access);
errno_t vgic400_distributor_icfgr(struct vgic400 *vgic, const struct vpc_memory_access *access, uintptr_t reg);

uint32_t vgic400_quad_byte_mask(const struct vgic400 *vgic, uint32_t irq);
uint64_t vgic400_p2v_cpu_map_b(uint64_t src, const struct vm *vm);
uint64_t vgic400_p2v_cpu_map_w(uint64_t src, const struct vm *vm);
uint64_t vgic400_v2p_cpu_map_b(uint64_t src, const struct vm *vm);
uint64_t vgic400_v2p_cpu_map_w(uint64_t src, const struct vm *vm);

void vgic400_distributor_error(const struct vpc_memory_access *access, const char *msg);

#ifdef __cplusplus
}
#endif

#endif /* ASSEMBLY */

#endif /* EMULATOR_VGIC400_VGIC400_LOCAL_H */

