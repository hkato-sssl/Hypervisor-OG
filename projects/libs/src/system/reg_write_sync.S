/*
 * system/reg_write_sync.S
 *
 * (C) 2020 Hidekazu Kato
 */
        .global         reg_write64_sync
        .global         reg_write32_sync
        .global         reg_write16_sync
        .global         reg_write8_sync

        .section        ".text.reg_write64_sync", "ax", %progbits
        .type           reg_write64_sync, %function
        .balign         4
reg_write64_sync:
        dsb             sy
        str             x2, [x0, x1]
        dsb             sy
        ldr             x0, [x0, x1]
        dsb             nsh
        ret

        .section        ".text.reg_write32_sync", "ax", %progbits
        .type           reg_write32_sync, %function
        .balign         4
reg_write32_sync:
        dsb             sy
        str             w2, [x0, x1]
        dsb             sy
        ldr             w0, [x0, x1]
        dsb             nsh
        ret

        .section        ".text.reg_write16_sync", "ax", %progbits
        .type           reg_write16_sync, %function
        .balign         4
reg_write16_sync:
        dsb             sy
        strh            w2, [x0, x1]
        dsb             sy
        ldrh            w0, [x0, x1]
        dsb             nsh
        ret

        .section        ".text.reg_write8_sync", "ax", %progbits
        .type           reg_write8_sync, %function
        .balign         4
reg_write8_sync:
        dsb             sy
        strb            w2, [x0, x1]
        dsb             sy
        ldrb            w0, [x0, x1]
        dsb             nsh
        ret

        .end

