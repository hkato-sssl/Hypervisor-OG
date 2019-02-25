/*
 * aarch64/mmu/aarch64_mmu_memclr.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"
#include "lib/bit.h"
#include "lib/system/errno.h"
#include "mmu_local.h"

        .global         aarch64_mmu_memclr
        .extern         memset

        .section        ".text.aarch64_mmu_memclr", "ax", %progbits
        .type           aarch64_mmu_memclr, %function
        .balign         4
        /*
         * x0 : address
         * x1 : region size
         */
aarch64_mmu_memclr:
        cbz             x0,  error
        tst             x0,  #(MMU_BLOCK_SZ - 1)
        b.ne            error
        cbz             x1,  error
        tst             x1,  #(MMU_BLOCK_SZ - 1)
        b.ne            error

        /* call memset() if the instruction is prohibited. */
        mrs             x2,  DCZID_EL0
        tbz             x2,  #4,  0f
        adr             ip0, memset
        br              ip0
0:
        mrs             x2,  DCZID_EL0
        and             x2,  x2,  #BITS(3, 0)
        mov             x3,  #4
        lsl             x3,  x3,  x2

        /*
         * x0 : address
         * x1 : region size
         * x3 : clear size
         */
clear_loop:
        dc              zva, x0
        add             x0,  x0,  x3
        subs            x1,  x1,  x3
        b.ne            clear_loop

        mov             x0,  #0
        ret

error:
        mov             x0,  #-EINVAL
        ret

        .end

