/*
 * emulator/vgic400/vgic400_cpu_map.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "hypervisor/vm.h"
#include "hypervisor/emulator/vgic400.h"
#include "vgic400_local.h"

/* defines */

/* types */

/* prototypes */

/* variables */

/* functions */

static uint64_t translate_cpu_map_b(uint64_t src, const uint8_t *table)
{
    int i;
    uint64_t d;

    d = 0;
    for (i = 0; i < MAX_NR_VM_PROCESSORS; ++i) {
        if ((src & BIT(i)) != 0) {
            d |= BIT(table[i]);
        }
    }

    return d;
}

static uint64_t translate_cpu_map_w(uint64_t src, const uint8_t *table)
{
    uint64_t d;
    uint64_t d0, d1, d2, d3;

    d0 = translate_cpu_map_b((src & BITS(7, 0)), table);
    src >>= 8;
    d1 = translate_cpu_map_b((src & BITS(7, 0)), table);
    src >>= 8;
    d2 = translate_cpu_map_b((src & BITS(7, 0)), table);
    src >>= 8;
    d3 = translate_cpu_map_b((src & BITS(7, 0)), table);
    d = (d3 << 24) | (d2 << 16) | (d1 << 8) | d0;

    return d;
}

uint64_t vgic400_v2p_cpu_map_b(uint64_t src, const struct vm *vm)
{
    uint64_t d;

    d = translate_cpu_map_b(src, vm->proc_map.physical);

    return d;
}

uint64_t vgic400_p2v_cpu_map_b(uint64_t src, const struct vm *vm)
{
    uint64_t d;

    d = translate_cpu_map_b(src, vm->proc_map.virtual);

    return d;
}

uint64_t vgic400_v2p_cpu_map_w(uint64_t src, const struct vm *vm)
{
    uint64_t d;

    d = translate_cpu_map_w(src, vm->proc_map.physical);

    return d;
}

uint64_t vgic400_p2v_cpu_map_w(uint64_t src, const struct vm *vm)
{
    uint64_t d;

    d = translate_cpu_map_w(src, vm->proc_map.virtual);

    return d;
}

