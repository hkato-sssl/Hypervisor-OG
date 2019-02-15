/*
 * aarch64/atomic.S
 *
 * (C) 2019 Hidekazu Kato
 */
        .global         atomic_add_u64
        .global         atomic_sub_u64

        .section        ".text.atomic_add_u64", "ax", %progbits
        .type           atomic_add_u64, %function
        .balign         4
atomic_add_u64:
        cbz             x0,  999f
0:
        ldxr            x2,  [x0]
        add             x2,  x2,  x1
        stxr            w3,  x2, [x0]
        cbnz            w3,  0b
999:
        ret

        .section        ".text.atomic_sub_u64", "ax", %progbits
        .type           atomic_add_u64, %function
        .balign         4
atomic_sub_u64:
        cbz             x0,  999f
0:
        ldxr            x2,  [x0]
        sub             x2,  x2,  x1
        stxr            w3,  x2, [x0]
        cbnz            w3,  0b
999:
        ret

        .end

