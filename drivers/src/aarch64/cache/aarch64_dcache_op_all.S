/*
 * aarch64/cache/aarch64_dcache_op_all.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"
#include "lib/bit.h"

        .global         aarch64_dcache_invd_all
        .global         aarch64_dcache_clean_all
        .global         aarch64_dcache_clean_invd_all

        .section        ".text.aarch64_dcache_invd_all", "ax", %progbits
        .type           aarch64_dcache_invd_all, %function
        .balign         4
aarch64_dcache_invd_all:
        adr             x0,  op_invd
        b               aarch64_dcache_op_all

op_invd:
        dc              isw, x0
        ret

        .section        ".text.aarch64_dcache_clean_all", "ax", %progbits
        .type           aarch64_dcache_clean_all, %function
        .balign         4
aarch64_dcache_clean_all:
        adr             x0, op_clean
        b               aarch64_dcache_op_all

op_clean:
        dc              csw, x0
        ret

        .section        ".text.aarch64_dcache_clean_invd_all", "ax", %progbits
        .type           aarch64_dcache_clean_invd_all, %function
        .balign         4
aarch64_dcache_clean_invd_all:
        adr             x0, op_clean_invd
        b               aarch64_dcache_op_all

op_clean_invd:
        dc              cisw, x0
        ret

        .section        ".text.aarch64_dcache_op_all", "ax", %progbits
        .type           aarch64_dcache_op_all, %function
        .balign         4
        /*
         * x0 - the maintenance function
         */
aarch64_dcache_op_all:
        dsb             osh

        mov             ip0, x0
        mov             ip1, lr

        // ip0 : the maintenance function
        // ip1 : old LR

        mrs             x9,  CLIDR_EL1
        lsr             x10, x9,  #(24 - 1)
        and             x10, x10, #BITS(3, 1)
        cbz             x10, exit

        // x9  : CLIDR_EL1
        // x10 : LoC << 1
loop_level:
        msr             CSSELR_EL1, x1
        isb
        mrs             x11, CCSIDR_EL1
        // CAUTION:
        // CCSIDR_EL1はARMv8.3-CCIDXの実装有無でフォーマットが
        // 変更される為、利用するプロセッサ仕様に注意が必要。
        and             x12, x11, #BITS(2, 0)
        add             x12, x12, #4
        ubfx            x13, x11, #3,  #10
        ubfx            x14, x11, #13, #15

        // x11 : CSSIDR_EL1
        // x12 : log2(number of bytes in cache line)
        // x13 : (aasociativity of cache) - 1
        // x14 : (numner of sets) - 1

        clz             w15, w13

        // x15 : (32 - A) = 32 - log2(ASSOSIATIVITY)

        sub             x1,  x10, #(1 << 1)
        mov             x2,  x14
        mov             x5,  #1
        lsl             x5,  x5,  x15
loop_set:
        lsl             x4,  x2,  x12
        orr             x4,  x4,  x1
        mov             x3,  x13
        // x1  : (index of cache level) << 1
        // x2  : index of set
        // x3  : index of associativity
        // x4  : part of SetWay
        lsl             x0,  x3,  x15
        orr             x0,  x0,  x4
loop_way:
        blr             ip0
        subs            x0,  x0,  x5
        b.cc            loop_way

        subs            x2,  x2,  #1
        b.cc            loop_set

        subs            x1,  x1,  #(1 << 1)
        b.cc            loop_level
exit:
        dsb             osh
        br              ip1

        .end
