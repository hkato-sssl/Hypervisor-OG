        .global         aarch64_write_mair
        .global         aarch64_read_mair
        .global         aarch64_write_amair
        .global         aarch64_read_amair
        .global         aarch64_write_tcr
        .global         aarch64_read_tcr
        .global         aarch64_write_ttbr0
        .global         aarch64_read_ttbr0
        .global         aarch64_write_sctlr
        .global         aarch64_read_sctlr

        .macro          BR_EL, table
        adr             ip0, \table
        mrs             ip1, CurrentEL
        add             ip0, ip0, ip1, lsl #1
        ldr             ip1, [ip0]
        br              ip1
        .endm

        .macro          ILLEGAL_OP
        svc             #0xffff
        .endm

        .section        ".text.aarch64_write_mair"
        .type           aarch64_write_mair, %function
        .balign         4
aarch64_write_mair:
        BR_EL           000f
        .balign         8
000:    .quad           999f
        .quad           write_mair_el1
        .quad           write_mair_el2
        .quad           write_mair_el3
999:
        ILLEGAL_OP
write_mair_el1:
        msr             MAIR_EL1, x0
        ret
write_mair_el2:
        msr             MAIR_EL2, x0
        ret
write_mair_el3:
        msr             MAIR_EL3, x0
        ret

        .section        ".text.aarch64_read_mair"
        .type           aarch64_read_mair, %function
        .balign         4
aarch64_read_mair:
        BR_EL           000f
        .balign         8
000:    .quad           999f
        .quad           read_mair_el1
        .quad           read_mair_el2
        .quad           read_mair_el3
999:
        ILLEGAL_OP
read_mair_el1:
        mrs             x0, MAIR_EL1
        ret
read_mair_el2:
        mrs             x0, MAIR_EL2
        ret
read_mair_el3:
        mrs             x0, MAIR_EL3
        ret

        .section        ".text.aarch64_write_amair"
        .type           aarch64_write_amair, %function
        .balign         4
aarch64_write_amair:
        BR_EL           000f
        .balign         8
000:    .quad           999f
        .quad           write_amair_el1
        .quad           write_amair_el2
        .quad           write_amair_el3
999:
        ILLEGAL_OP
write_amair_el1:
        msr             AMAIR_EL1, x0
        ret
write_amair_el2:
        msr             AMAIR_EL2, x0
        ret
write_amair_el3:
        msr             AMAIR_EL3, x0
        ret

        .section        ".text.aarch64_read_amair"
        .type           aarch64_read_amair, %function
        .balign         4
aarch64_read_amair:
        BR_EL           000f
        .balign         8
000:    .quad           999f
        .quad           read_amair_el1
        .quad           read_amair_el2
        .quad           read_amair_el3
999:
        ILLEGAL_OP
read_amair_el1:
        mrs             x0, AMAIR_EL1
        ret
read_amair_el2:
        mrs             x0, AMAIR_EL2
        ret
read_amair_el3:
        mrs             x0, AMAIR_EL3
        ret

        .section        ".text.aarch64_write_tcr"
        .type           aarch64_write_tcr, %function
        .balign         4
aarch64_write_tcr:
        BR_EL           000f
        .balign         8
000:    .quad           999f
        .quad           write_tcr_el1
        .quad           write_tcr_el2
        .quad           write_tcr_el3
999:
        ILLEGAL_OP
write_tcr_el1:
        msr             TCR_EL1, x0
        ret
write_tcr_el2:
        msr             TCR_EL2, x0
        ret
write_tcr_el3:
        msr             TCR_EL3, x0
        ret

        .section        ".text.aarch64_read_tcr"
        .type           aarch64_read_tcr, %function
        .balign         4
aarch64_read_tcr:
        BR_EL           000f
        .balign         8
000:    .quad           999f
        .quad           read_tcr_el1
        .quad           read_tcr_el2
        .quad           read_tcr_el3
999:
        ILLEGAL_OP
read_tcr_el1:
        mrs             x0, TCR_EL1
        ret
read_tcr_el2:
        mrs             x0, TCR_EL2
        ret
read_tcr_el3:
        mrs             x0, TCR_EL3
        ret

        .section        ".text.aarch64_write_ttbr0"
        .type           aarch64_write_ttbr0, %function
        .balign         4
aarch64_write_ttbr0:
        BR_EL           000f
        .balign         8
000:    .quad           999f
        .quad           write_ttbr0_el1
        .quad           write_ttbr0_el2
        .quad           write_ttbr0_el3
999:
        ILLEGAL_OP
write_ttbr0_el1:
        msr             TTBR0_EL1, x0
        ret
write_ttbr0_el2:
        msr             TTBR0_EL2, x0
        ret
write_ttbr0_el3:
        msr             TTBR0_EL3, x0
        ret

        .section        ".text.aarch64_read_ttbr0"
        .type           aarch64_read_ttbr0, %function
        .balign         4
aarch64_read_ttbr0:
        BR_EL           000f
        .balign         8
000:    .quad           999f
        .quad           read_ttbr0_el1
        .quad           read_ttbr0_el2
        .quad           read_ttbr0_el3
999:
        ILLEGAL_OP
read_ttbr0_el1:
        mrs             x0, TTBR0_EL1
        ret
read_ttbr0_el2:
        mrs             x0, TTBR0_EL2
        ret
read_ttbr0_el3:
        mrs             x0, TTBR0_EL3
        ret

        .section        ".text.aarch64_write_sctlr"
        .type           aarch64_write_sctlr, %function
        .balign         4
aarch64_write_sctlr:
        BR_EL           000f
        .balign         8
000:    .quad           999f
        .quad           write_sctlr_el1
        .quad           write_sctlr_el2
        .quad           write_sctlr_el3
999:
        ILLEGAL_OP
write_sctlr_el1:
        msr             SCTLR_EL1, x0
        ret
write_sctlr_el2:
        msr             SCTLR_EL2, x0
        ret
write_sctlr_el3:
        msr             SCTLR_EL3, x0
        ret

        .section        ".text.aarch64_read_sctlr"
        .type           aarch64_read_sctlr, %function
        .balign         4
aarch64_read_sctlr:
        BR_EL           000f
        .balign         8
000:    .quad           999f
        .quad           read_sctlr_el1
        .quad           read_sctlr_el2
        .quad           read_sctlr_el3
999:
        ILLEGAL_OP
read_sctlr_el1:
        mrs             x0, SCTLR_EL1
        ret
read_sctlr_el2:
        mrs             x0, SCTLR_EL2
        ret
read_sctlr_el3:
        mrs             x0, SCTLR_EL3
        ret

        .end

