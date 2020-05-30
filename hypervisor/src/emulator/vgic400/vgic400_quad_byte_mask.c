/*
 * emulator/vgic400/vgic400_quad_byte_mask.c
 *
 * (C) 2019 Hidekazu Kato
 */

#include <stdint.h>
#include "hypervisor/emulator/vgic400.h"

/* defines */

/* types */

/* prototypes */

/* variables */

static const uint32_t table[] = {
    0x00000000,     /* 0000b */
    0x000000ff,     /* 0001b */
    0x0000ff00,     /* 0010b */
    0x0000ffff,     /* 0011b */
    0x00ff0000,     /* 0100b */
    0x00ff00ff,     /* 0101b */
    0x00ffff00,     /* 0110b */
    0x00ffffff,     /* 0111b */
    0xff000000,     /* 1000b */
    0xff0000ff,     /* 1001b */
    0xff00ff00,     /* 1010b */
    0xff00ffff,     /* 1011b */
    0xffff0000,     /* 1100b */
    0xffff00ff,     /* 1101b */
    0xffffff00,     /* 1110b */
    0xffffffff      /* 1111b */
};

/* functions */

uint32_t vgic400_quad_byte_mask(struct vgic400 *vgic, uint32_t virq)
{
    uint32_t mask;

    mask = vgic->target.virq[virq / 32];
    mask = (mask >> (virq & 0x1f)) & 0x0f;
    mask = table[mask];

    return mask;
}

