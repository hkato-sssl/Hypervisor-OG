/*
 * aarch64/atomic.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         aarch64_atomic_add_u64
        .global         aarch64_atomic_sub_u64
        .global         aarch64_atomic_swap_u8

        .section        ".text.aarch64_atomic_add_u64", "ax", %progbits
        .type           aarch64_atomic_add_u64, %function
        .balign         4
aarch64_atomic_add_u64:
0:
        ldxr            x2,  [x0]
        add             x2,  x2,  x1
        stxr            w3,  x2, [x0]
        cbnz            w3,  0b

        ret

        .section        ".text.aarch64_atomic_sub_u64", "ax", %progbits
        .type           aarch64_atomic_sub_u64, %function
        .balign         4
aarch64_atomic_sub_u64:
0:
        ldxr            x2,  [x0]
        sub             x2,  x2,  x1
        stxr            w3,  x2, [x0]
        cbnz            w3,  0b

        ret

        .section        ".text.aarch64_atomic_swap_u8", "ax", %progbits
        .type           aarch64_atomic_swap_u8, %function
        .balign         4
aarch64_atomic_swap_u8:
0:
        ldxrb           w2,  [x0]
        stxrb           w3,  w1, [x0]
        cbnz            w3,  0b

        mov             x0,  x2
        ret

        .end

