/*
 * aarch64/aarch64_udelay.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"
#include "lib/bit.h"

#define CONFIG_PCT_WIDTH        64      /* width of a physical timer */

#if (CONFIG_PCT_WIDTH >= 56) && (CONFIG_PCT_WIDTH < 64)
#define PCT_MASK        BITS(63, CONFIG_PCT_WIDTH)
#elif CONFIG_PCT_WIDTH == 64
#define PCT_MASK        0
#else
#error CONFIG_PCT_WIDTH has bees set to illegal value.
#endif

        .global         aarch64_udelay

        .section        ".text.aarch64_udelay", "ax", %progbits
        .type           aarch64_udelay, %function
        .balign         4

        /*
         * void aarch64_udelay(uint32_t usec)
         */
aarch64_udelay:
        isb
        mrs             x1, CNTPCT_EL0
        add             x1, x1, #1
        mrs             x2, CNTFRQ_EL0          // CNTFRQ_EL0 is 32-bit register
        cbz             w0, exit

        mov32           w3, 1000000
        udiv            w2, w2, w3
        umull           x2, w2, w0
        mov             x3, #PCT_MASK
loop:
        isb
        mrs             x0, CNTPCT_EL0
        sub             x0, x0, x1
        bic             x0, x0, x3
        cmp             x2, x0
        b.cc            exit
        yield
        b               loop
exit:
        ret

        .end

