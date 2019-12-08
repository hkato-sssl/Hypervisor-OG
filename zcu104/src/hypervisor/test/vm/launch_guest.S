/*
 * hypervisor/test/vm/launch_guest.S
 *
 * (C) 2019 Hidekazu Kato
 */

#include "lib/asm.h"

        .global         test_vm_launch_guest_start

        .section        ".text.test_vm_launch_guest_start", "ax", %progbits
        .type           test_vm_launch_guest_start, %function
        .balign         4
test_vm_launch_guest_start:
        mov             x1, #0x1234
        mov32           x2, 0xa0001000
        mov             x3, #'@'

        bl              put_char
        bl              put_char
        bl              put_char
        bl              put_char

        mov32           x15, 0xffffaaaa
        hvc             #0

put_char:
        ldr             w0, [x2, #8]
        ands            w0, w0, #(1 << 2)
        beq             put_char
        str             w3, [x2, #4]
        dsb             sy
        ret

        .end

