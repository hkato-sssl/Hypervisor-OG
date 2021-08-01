/*
 * test/hvcs/01_asm.S
 *
 * (C) 2020 Hidekazu Kato
 */
        .global         test_hvcs_send
        .global         test_hvcs_recv

#define SERVICE_ID      (('P' << 56) | ('1' << 48) | ('2' << 40) | ('8' << 32))
#define CMD_SEND        (('W' << 24) | ('R' << 16))
#define CMD_RECV        (('R' << 24) | ('D' << 16))

        .section        ".text.test_hvcs_send", "ax", %progbits
        .type           test_hvcs_send, %function
        .balign         4
test_hvcs_send:
        ldr             ip0, =(SERVICE_ID | CMD_SEND)
        bfi             ip0, x0,  #0, #8

        mov             ip1, x1
        ldp             x0,  x1,  [ip1], #16
        ldp             x2,  x3,  [ip1], #16
        ldp             x4,  x5,  [ip1], #16
        ldp             x6,  x7,  [ip1], #16
        ldp             x8,  x9,  [ip1], #16
        ldp             x10, x11, [ip1], #16
        ldp             x12, x13, [ip1], #16
        ldp             x14, x15, [ip1]
        hvc             #1
        ret

        .section        ".text.test_hvcs_recv", "ax", %progbits
        .type           test_hvcs_recv, %function
        .balign         4
test_hvcs_recv:
        str             x1,  [sp, #-16]!

        ldr             ip0, =(SERVICE_ID | CMD_RECV)
        bfi             ip0, x0,  #0, #8
        hvc             #1

        ldr             ip1, [sp], #16
        cbnz            x0, 0f

        stp             x1,  x2,  [ip1], #16
        stp             x3,  x4,  [ip1], #16
        stp             x5,  x6,  [ip1], #16
        stp             x7,  x8,  [ip1], #16
        stp             x9,  x10, [ip1], #16
        stp             x11, x12, [ip1], #16
        stp             x13, x14, [ip1], #16
        stp             x15, ip0, [ip1], #16
0:
        ret

        .end

