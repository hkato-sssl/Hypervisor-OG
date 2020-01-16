/*
 * test/insn/01_parse_aarch64_ldr.S
 *
 * (C) 2020 Hidekazu Kato
 */

/* テスト内容：AArch64 LDR命令の解析結果確認
 *
 * Hypervisorがエミュレーションをサポートするメモリアクセス命令を実行した
 * 際の解析結果を確認する。その為、実行内容に意味は無い。
 *
 * 1.解析対象命令
 * Hypervisorが解析を実行するLDR命令は以下の通り。
 * (4)の様なliteralを用いる形式の実行にはメモリマッピングの調整が必要とな
 * る為、別途テスト環境を用意する。
 *
 * LDR (64-bit)
 * (1) LDR Xn, [<Xt|SP>], #simm9
 * (2) LDR Xn, [<Xt|SP>, #simm9]!
 * (3) LDR Xn, [<Xt|SP>{, #pimm12}]
 * (4) LDR Xn, <label>
 * (5) LDR Xn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]
 * (6) LDUR Xn, [<Xt|SP>{, #simm9}]
 *
 * LDR (32-bit)
 * (1) LDR Wn, [<Xt|SP>], #simm9
 * (2) LDR Wn, [<Xt|SP>, #simm9]!
 * (3) LDR Wn, [<Xt|SP>{, #pimm12}]
 * (4) LDR Wn, <label>
 * (5) LDR Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]
 * (6) LDUR Wn, [<Xt|SP>{, #simm9}]
 * (7) LDRSW Xn, [<Xt|SP>], #simm9
 * (8) LDRSW Xn, [<Xt|SP>, #simm9]!
 * (9) LDRSW Xn, [<Xt|SP>{, #pimm12}]
 * (10) LDRSW Xn, <label>
 * (11) LDRSW Xn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]
 * (12) LDURSW Xn, [<Xt|SP>{, #simm9}]
 *
 * LDR (16-bit)
 * (1) LDRH Wn, [<Xt|SP>], #simm9
 * (2) LDRH Wn, [<Xt|SP>, #simm9]!
 * (3) LDRH Wn, [<Xt|SP>{, #pimm12}]
 * (4) LDRH Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]
 * (5) LDURH Wn, [<Xt|SP>{, #simm9}]
 * (6) LDRSH Xn, [<Xt|SP>], #simm9
 * (7) LDRSH Xn, [<Xt|SP>, #simm9]!
 * (8) LDRSH Xn, [<Xt|SP>{, #pimm12}]
 * (9) LDRSH Xn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]
 * (10) LDURSH Xn, [<Xt|SP>{, #simm9}]
 * (11) LDRSH Wn, [<Xt|SP>], #simm9
 * (12) LDRSH Wn, [<Xt|SP>, #simm9]!
 * (13) LDRSH Wn, [<Xt|SP>{, #pimm12}]
 * (14) LDRSH Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]
 * (15) LDURSH Wn, [<Xt|SP>{, #simm9}]
 *
 * LDR (8-bit)
 * (1) LDRB Wn, [<Xt|SP>], #simm9
 * (2) LDRB Wn, [<Xt|SP>, #simm9]!
 * (3) LDRB Wn, [<Xt|SP>{, #pimm12}]
 * (4) LDRB Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]
 * (5) LDURB Wn, [<Xt|SP>{, #simm9}]
 * (6) LDRSB Xn, [<Xt|SP>], #simm9
 * (7) LDRSB Xn, [<Xt|SP>, #simm9]!
 * (8) LDRSB Xn, [<Xt|SP>{, #pimm12}]
 * (9) LDRSB Xn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]
 * (10) LDURSB Xn, [<Xt|SP>{, #simm9}]
 * (11) LDRSB Wn, [<Xt|SP>], #simm9
 * (12) LDRSB Wn, [<Xt|SP>, #simm9]!
 * (13) LDRSB Wn, [<Xt|SP>{, #pimm12}]
 * (14) LDRSB Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]
 * (15) LDURSB Wn, [<Xt|SP>{, #simm9}]
 *
 * 各パターンの(1),(2),(7),(8)はHypervisorが直接Instruction Codeを
 * 解析している。それ以外はプロセッサが解析結果をSystem Registerに
 * 設定する。
 *
 * 2.テスト実施方針
 * テストで確認するポイントは２つ
 * (a) (1),(2),(7),(8)のパターンを正確に解析できているか
 * (b) 上記以外はプロセッサの解析結果を正確に処理できているか
 *
 * (a)に関してはInstruction parserが様々なレジスタ指定形式に対応し
 * ているかを確認し、(b)ではSystem Registerに解析結果が設定される
 * 事を確認する。確認事項が異なる為、(b)は(a)より試験項目を少なく
 * している。
 *
 * 3.HVC命令の利用方法
 * テスト項目の節目を示す為にHVC命令を実行している。実行形式は下記
 * の通り。
 *
 * hvc #0xSSNN  // SS=処理サイズ、NN=項目番号
 *
 */

#include "config/system.h"
#include "lib/asm.h"

#define TEST_NO(s, n)   (((s) << 8) | (n))

        .macro          func_entry
        str             lr, [sp, #-16]!
        mov             x0, sp
        msr             TPIDR_EL1, x0
        .endm

        .macro          func_exit
        mrs             x0, TPIDR_EL1
        mov             sp, x0
        ldr             lr, [sp], #16
        .endm

        .macro          test_entry,no
        bl              init_regs
        mov             lr, x1
        hvc             #\no
        .endm

        .global         test_insn_01

        .section        ".text.test_insn_01", "ax", %progbits
        .type           test_insn_01, %function
        .balign         4
test_insn_01:
        str             lr, [sp, #-16]!

        bl              test_insn_01_ldr64
        bl              test_insn_01_ldr32
        bl              test_insn_01_ldr16
        bl              test_insn_01_ldr8

        ldr             lr, [sp], #16
        ret

init_regs:
        mov64           x0, CONFIG_REGION_TRAP
        mov             x1, x0
        mov             x2, x0
        mov             x3, x0
        mov             x4, x0
        mov             x5, x0
        mov             x6, x0
        mov             x7, x0
        mov             x8, x0
        mov             x9, x0
        mov             x10, x0
        mov             x11, x0
        mov             x12, x0
        mov             x13, x0
        mov             x14, x0
        mov             x15, x0
        mov             x16, x0
        mov             x17, x0
        mov             x18, x0
        mov             x19, x0
        mov             x20, x0
        mov             x21, x0
        mov             x22, x0
        mov             x23, x0
        mov             x24, x0
        mov             x25, x0
        mov             x26, x0
        mov             x27, x0
        mov             x28, x0
        mov             x29, x0
        mov             sp, x0
        ret

        .section        ".text.test_insn_01_ldr64", "ax", %progbits
        .type           test_insn_01_ldr64, %function
        .balign         4
test_insn_01_ldr64:
        func_entry

        // (1) LDR Xn, [<Xt|SP>], #simm9

        test_entry      TEST_NO(64, 1)

        ldr             x0, [x0], #8
        ldr             x0, [x1], #8
        ldr             x7, [x1], #8
        ldr             x30, [x1], #8
        ldr             xzr, [x1], #8

        ldr             x0, [x9], #8
        ldr             x0, [x8], #8
        ldr             x0, [x10], #8
        ldr             x0, [SP], #8

        ldr             x0, [x1], #0
        ldr             x0, [x1], #1
        ldr             x0, [x1], #128
        ldr             x0, [x1], #255
        ldr             x0, [x1], #-256

        // (2) LDR Xn, [<Xt|SP>, #simm9]!

        test_entry      TEST_NO(64, 2)

        add             x2, x1, #0x10000

        ldr             x0, [x0, #8]!
        ldr             x0, [x1, #8]!
        ldr             x7, [x1, #8]!
        ldr             x30, [x1, #8]!
        ldr             xzr, [x1, #8]!

        ldr             x0, [x0, #8]!
        ldr             x0, [x9, #8]!
        ldr             x0, [x30, #8]!
        ldr             x0, [sp, #8]!

        ldr             x0, [x1, #0]!
        ldr             x0, [x1, #128]!
        ldr             x0, [x1, #248]!
        ldr             x0, [x2, #-256]!

        // (3) LDR Xn, [<Xt|SP>{, #pimm12}]

        test_entry      TEST_NO(64, 3)

        ldr             x0, [x1, #8]
        ldr             x7, [x1, #8]
        ldr             x30, [x1, #8]
        ldr             xzr, [x1, #8]

        ldr             x0, [x0, #8]
        ldr             x0, [x7, #8]
        ldr             x0, [x30, #8]
        ldr             x0, [sp, #8]

        ldr             x0, [x0, #0]
        ldr             x0, [x0, #256]
        ldr             x0, [x0, #32760]

        // (5) LDR Xn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        test_entry      TEST_NO(64, 5)

        add             x2, x1, #0x20000
        mov64           x3, 0x0000000000012340
        mov64           x4, 0xffffffff00012340
        mov64           x5, 0xffffffffffffff00
        mov64           x6, 0x00000000ffffff00

        ldr             x0, [sp, x3]
        ldr             x0, [x8, w4, uxtw]
        ldr             x0, [x2, x5, lsl #0]
        ldr             x0, [x2, x5, lsl #3]
        ldr             xzr, [x2, w6, sxtw]
        ldr             xzr, [x12, x3, sxtx]

        // (6) LDUR Xn, [<Xt|SP>{, #simm9}]

        test_entry      TEST_NO(64, 6)

        add             x2, x1, #0x10000

        ldur            x0, [x8, #0]
        ldur            x0, [sp, #248]
        ldur            xzr, [x2, #-256]

        func_exit
        ret

        .section        ".text.test_insn_01_ldr32", "ax", %progbits
        .type           test_insn_01_ldr32, %function
        .balign         4
test_insn_01_ldr32:
        func_entry

        // (1) LDR Wn, [<Xt|SP>], #simm9

        test_entry      TEST_NO(32, 1)

        ldr             w0, [x0], #8
        ldr             w0, [x1], #8
        ldr             w7, [sp], #8
        ldr             wzr, [x1], #8
        ldr             wzr, [x1], #0
        ldr             wzr, [x1], #255
        ldr             wzr, [x1], #-256

        // (2) LDR Wn, [<Xt|SP>, #simm9]!

        test_entry      TEST_NO(32, 2)

        add             x2, x1, #0x20000

        ldr             w0, [x0, #8]!
        ldr             w0, [x1, #8]!
        ldr             w15, [sp, #8]!
        ldr             wzr, [x8, #8]!
        ldr             wzr, [x2, #0]!
        ldr             wzr, [x2, #252]!
        ldr             wzr, [x2, #-256]!

        // (3) LDR Wn, [<Xt|SP>{, #pimm12}]

        test_entry      TEST_NO(32, 3)

        ldr             w0, [x1, #0]
        ldr             wzr, [x1, #256]
        ldr             w20, [sp, #16380]

        // (5) LDR Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        test_entry      TEST_NO(32, 5)

        add             x2, x1, #0x20000
        mov64           x3, 0x0000000000012340
        mov64           x4, 0xffffffff00012340
        mov64           x5, 0xffffffffffffff00
        mov64           x6, 0x00000000ffffff00

        ldr             w0, [sp, x3]
        ldr             w0, [x8, w4, uxtw]
        ldr             w0, [x2, x5, lsl #0]
        ldr             w0, [x2, x5, lsl #2]
        ldr             wzr, [x2, w6, sxtw]
        ldr             wzr, [x12, x3, sxtx]

        // (6) LDUR Wn, [<Xt|SP>{, #simm9}]

        test_entry      TEST_NO(32, 6)

        add              x2, x1, #0x20000

        ldur             w0, [x8, #0]
        ldur             w0, [sp, #252]
        ldur             wzr, [x2, #-256]

        // (7) LDRSW Xn, [<Xt|SP>], #simm9

        test_entry      TEST_NO(32, 7)

        ldrsw           x0, [x0], #8
        ldrsw           x0, [x1], #8
        ldrsw           xzr, [x1], #255
        ldrsw           x30, [sp], #-256

        // (8) LDRSW Xn, [<Xt|SP>, #simm9]!

        test_entry      TEST_NO(32, 8)

        add             x2, x1, #0x20000

        ldrsw           x0, [x0, #8]!
        ldrsw           x0, [x1, #8]!
        ldrsw           x15, [sp, #8]!
        ldrsw           xzr, [x8, #8]!
        ldrsw           xzr, [x2, #0]!
        ldrsw           xzr, [x2, #252]!
        ldrsw           xzr, [x2, #-256]!

        // (9) LDRSW Xn, [<Xt|SP>{, #pimm12}]

        test_entry      TEST_NO(32, 9)

        ldrsw           x0, [x1, #0]
        ldrsw           xzr, [x1, #256]
        ldrsw           x20, [sp, #16380]

        // (11) LDRSW Xn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        test_entry      TEST_NO(32, 11)

        add             x2, x1, #0x20000
        mov64           x3, 0x0000000000012340
        mov64           x4, 0xffffffff00012340
        mov64           x5, 0xffffffffffffff00
        mov64           x6, 0x00000000ffffff00

        ldrsw           x0, [sp, x3]
        ldrsw           x0, [x8, w4, uxtw]
        ldrsw           x0, [x2, x5, lsl #0]
        ldrsw           x0, [x2, x5, lsl #2]
        ldrsw           xzr, [x2, w6, sxtw]
        ldrsw           xzr, [x12, x3, sxtx]

        // (12) LDURSW Xn, [<Xt|SP>{, #simm9}]

        test_entry      TEST_NO(32, 12)

        add             x2, x1, #0x20000

        ldursw          x0, [x8, #0]
        ldursw          x0, [sp, #252]
        ldursw          xzr, [x2, #-256]

        func_exit
        ret

        .section        ".text.test_insn_01_ldr16", "ax", %progbits
        .type           test_insn_01_ldr16, %function
        .balign         4
test_insn_01_ldr16:
        func_entry

        // (1) LDRH Wn, [<Xt|SP>], #simm9

        test_entry      TEST_NO(16, 1)

        ldrh            w0, [x0], #8
        ldrh            w0, [x1], #8
        ldrh            w7, [sp], #8
        ldrh            wzr, [x1], #8
        ldrh            wzr, [x1], #0
        ldrh            wzr, [x1], #255
        ldrh            wzr, [x1], #-256

        // (2) LDRH Wn, [<Xt|SP>, #simm9]!

        test_entry      TEST_NO(16, 2)

        add             x2, x1, #0x20000

        ldrh            w0, [x0, #8]!
        ldrh            w0, [x1, #8]!
        ldrh            w15, [sp, #8]!
        ldrh            wzr, [x8, #8]!
        ldrh            wzr, [x2, #0]!
        ldrh            wzr, [x2, #252]!
        ldrh            wzr, [x2, #-256]!

        // (3) LDRH Wn, [<Xt|SP>{, #pimm12}]

        test_entry      TEST_NO(16, 3)

        ldrh            w0, [x1, #0]
        ldrh            wzr, [x1, #256]
        ldrh            w20, [sp, #8190]

        // (4) LDRH Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        test_entry      TEST_NO(16, 4)

        add             x2, x1, #0x20000
        mov64           x3, 0x0000000000012340
        mov64           x4, 0xffffffff00012340
        mov64           x5, 0xffffffffffffff00
        mov64           x6, 0x00000000ffffff00

        ldrh            w0, [sp, x3]
        ldrh            w0, [x8, w4, uxtw]
        ldrh            w0, [x2, x5, lsl #0]
        ldrh            w0, [x2, x5, lsl #1]
        ldrh            wzr, [x2, w6, sxtw]
        ldrh            wzr, [x12, x3, sxtx]

        // (5) LDURH Wn, [<Xt|SP>{, #simm9}]

        test_entry      TEST_NO(16, 5)

        add              x2, x1, #0x20000

        ldurh            w0, [x8, #0]
        ldurh            w0, [sp, #252]
        ldurh            wzr, [x2, #-256]

        // (6) LDRSH Xn, [<Xt|SP>], #simm9

        test_entry      TEST_NO(16, 6)

        ldrsh           x0, [x0], #8
        ldrsh           x0, [x1], #8
        ldrsh           xzr, [x1], #255
        ldrsh           x30, [sp], #-256

        // (7) LDRSH Xn, [<Xt|SP>, #simm9]!

        test_entry      TEST_NO(16, 7)

        add             x2, x1, #0x20000

        ldrsh           x0, [x0, #8]!
        ldrsh           x0, [x1, #8]!
        ldrsh           x15, [sp, #8]!
        ldrsh           xzr, [x8, #8]!
        ldrsh           xzr, [x2, #0]!
        ldrsh           xzr, [x2, #252]!
        ldrsh           xzr, [x2, #-256]!

        // (8) LDRSH Xn, [<Xt|SP>{, #pimm12}]

        test_entry      TEST_NO(16, 8)

        ldrsh           x0, [x1, #0]
        ldrsh           xzr, [x1, #256]
        ldrsh           x20, [sp, #8190]

        // (9) LDRSH Xn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        test_entry      TEST_NO(16, 9)

        add             x2, x1, #0x20000
        mov64           x3, 0x0000000000012340
        mov64           x4, 0xffffffff00012340
        mov64           x5, 0xffffffffffffff00
        mov64           x6, 0x00000000ffffff00

        ldrsh           x0, [sp, x3]
        ldrsh           x0, [x8, w4, uxtw]
        ldrsh           x0, [x2, x5, lsl #0]
        ldrsh           x0, [x2, x5, lsl #1]
        ldrsh           xzr, [x2, w6, sxtw]
        ldrsh           xzr, [x12, x3, sxtx]

        // (10) LDURSH Xn, [<Xt|SP>{, #simm9}]

        test_entry      TEST_NO(16, 10)

        add              x2, x1, #0x20000

        ldursh           x0, [x8, #0]
        ldursh           x0, [sp, #252]
        ldursh           xzr, [x2, #-256]

        // (11) LDRSH Wn, [<Xt|SP>], #simm9

        test_entry      TEST_NO(16, 11)

        ldrsh           w0, [x0], #8
        ldrsh           w0, [x1], #8
        ldrsh           wzr, [x1], #255
        ldrsh           w30, [sp], #-256

        // (12) LDRSH Wn, [<Xt|SP>, #simm9]!

        test_entry      TEST_NO(16, 12)

        add             x2, x1, #0x20000

        ldrsh           w0, [x0, #8]!
        ldrsh           w0, [x1, #8]!
        ldrsh           w15, [sp, #8]!
        ldrsh           wzr, [x8, #8]!
        ldrsh           wzr, [x2, #0]!
        ldrsh           wzr, [x2, #252]!
        ldrsh           wzr, [x2, #-256]!

        // (13) LDRSH Wn, [<Xt|SP>{, #pimm12}]

        test_entry      TEST_NO(16, 13)

        ldrsh           w0, [x1, #0]
        ldrsh           wzr, [x1, #256]
        ldrsh           w20, [sp, #8190]

        // (14) LDRSH Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        test_entry      TEST_NO(16, 14)

        add             x2, x1, #0x20000
        mov64           x3, 0x0000000000012340
        mov64           x4, 0xffffffff00012340
        mov64           x5, 0xffffffffffffff00
        mov64           x6, 0x00000000ffffff00

        ldrsh           w0, [sp, x3]
        ldrsh           w0, [x8, w4, uxtw]
        ldrsh           w0, [x2, x5, lsl #0]
        ldrsh           w0, [x2, x5, lsl #1]
        ldrsh           wzr, [x2, w6, sxtw]
        ldrsh           wzr, [x12, x3, sxtx]

        // (15) LDURSH Wn, [<Xt|SP>{, #simm9}]

        test_entry      TEST_NO(16, 15)

        add              x2, x1, #0x20000

        ldursh           w0, [x8, #0]
        ldursh           w0, [sp, #252]
        ldursh           wzr, [x2, #-256]

        func_exit
        ret

        .section        ".text.test_insn_01_ldr8", "ax", %progbits
        .type           test_insn_01_ldr8, %function
        .balign         4
test_insn_01_ldr8:
        func_entry

        // (1) LDRB Wn, [<Xt|SP>], #simm9

        test_entry      TEST_NO(8, 1)

        ldrb            w0, [x0], #8
        ldrb            w0, [x1], #8
        ldrb            w7, [sp], #8
        ldrb            wzr, [x1], #8
        ldrb            wzr, [x1], #0
        ldrb            wzr, [x1], #255
        ldrb            wzr, [x1], #-256

        // (2) LDRB Wn, [<Xt|SP>, #simm9]!

        test_entry      TEST_NO(8, 2)

        add             x2, x1, #0x20000

        ldrb            w0, [x0, #8]!
        ldrb            w0, [x1, #8]!
        ldrb            w15, [sp, #8]!
        ldrb            wzr, [x8, #8]!
        ldrb            wzr, [x2, #0]!
        ldrb            wzr, [x2, #252]!
        ldrb            wzr, [x2, #-256]!

        // (3) LDRB Wn, [<Xt|SP>{, #pimm12}]

        test_entry      TEST_NO(8, 3)

        ldrb            w0, [x1, #0]
        ldrb            wzr, [x1, #256]
        ldrb            w20, [sp, #4095]

        // (4) LDRB Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        test_entry      TEST_NO(8, 4)

        add             x2, x1, #0x20000
        mov64           x3, 0x0000000000012340
        mov64           x4, 0xffffffff00012340
        mov64           x5, 0xffffffffffffff00
        mov64           x6, 0x000000004095

        ldrb            w0, [sp, x3]
        ldrb            w0, [x8, w4, uxtw]
        ldrb            w0, [x2, x5, lsl #0]
        ldrb            wzr, [x2, w6, sxtw]
        ldrb            wzr, [x12, x3, sxtx]

        // (5) LDURB Wn, [<Xt|SP>{, #simm9}]

        test_entry      TEST_NO(8, 5)

        add              x2, x1, #0x20000

        ldurb            w0, [x8, #0]
        ldurb            w0, [sp, #252]
        ldurb            wzr, [x2, #-256]

        // (6) LDRSB Xn, [<Xt|SP>], #simm9

        test_entry      TEST_NO(8, 6)

        ldrsb           x0, [x0], #8
        ldrsb           x0, [x1], #8
        ldrsb           xzr, [x1], #255
        ldrsb           x30, [sp], #-256

        // (7) LDRSB Xn, [<Xt|SP>, #simm9]!

        test_entry      TEST_NO(8, 7)

        add             x2, x1, #0x20000

        ldrsb           x0, [x0, #8]!
        ldrsb           x0, [x1, #8]!
        ldrsb           x15, [sp, #8]!
        ldrsb           xzr, [x8, #8]!
        ldrsb           xzr, [x2, #0]!
        ldrsb           xzr, [x2, #252]!
        ldrsb           xzr, [x2, #-256]!

        // (8) LDRSB Xn, [<Xt|SP>{, #pimm12}]

        test_entry      TEST_NO(8, 8)

        ldrsb           x0, [x1, #0]
        ldrsb           xzr, [x1, #256]
        ldrsb           x20, [sp, #4095]

        // (9) LDRSB Xn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        test_entry      TEST_NO(8, 9)

        add             x2, x1, #0x20000
        mov64           x3, 0x0000000000012340
        mov64           x4, 0xffffffff00012340
        mov64           x5, 0xffffffffffffff00
        mov64           x6, 0x00000000ffffff00

        ldrsb           x0, [sp, x3]
        ldrsb           x0, [x8, w4, uxtw]
        ldrsb           x0, [x2, x5, lsl #0]
        ldrsb           xzr, [x2, w6, sxtw]
        ldrsb           xzr, [x12, x3, sxtx]

        // (10) LDURSB Xn, [<Xt|SP>{, #simm9}]

        test_entry      TEST_NO(8, 10)

        add              x2, x1, #0x20000

        ldursb           x0, [x8, #0]
        ldursb           x0, [sp, #252]
        ldursb           xzr, [x2, #-256]

        // (11) LDRSB Wn, [<Xt|SP>], #simm9

        test_entry      TEST_NO(8, 11)

        ldrsb           w0, [x0], #8
        ldrsb           w0, [x1], #8
        ldrsb           wzr, [x1], #255
        ldrsb           w30, [sp], #-256

        // (12) LDRSB Wn, [<Xt|SP>, #simm9]!

        test_entry      TEST_NO(8, 12)

        add             x2, x1, #0x20000

        ldrsb           w0, [x0, #8]!
        ldrsb           w0, [x1, #8]!
        ldrsb           w15, [sp, #8]!
        ldrsb           wzr, [x8, #8]!
        ldrsb           wzr, [x2, #0]!
        ldrsb           wzr, [x2, #252]!
        ldrsb           wzr, [x2, #-256]!

        // (13) LDRSB Wn, [<Xt|SP>{, #pimm12}]

        test_entry      TEST_NO(8, 13)

        ldrsb           w0, [x1, #0]
        ldrsb           wzr, [x1, #256]
        ldrsb           w20, [sp, #4095]

        // (14) LDRSB Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        test_entry      TEST_NO(8, 14)

        add             x2, x1, #0x20000
        mov64           x3, 0x0000000000012340
        mov64           x4, 0xffffffff00012340
        mov64           x5, 0xffffffffffffff00
        mov64           x6, 0x00000000ffffff00

        ldrsb           w0, [sp, x3]
        ldrsb           w0, [x8, w4, uxtw]
        ldrsb           w0, [x2, x5, lsl #0]
        ldrsb           wzr, [x2, w6, sxtw]
        ldrsb           wzr, [x12, x3, sxtx]

        // (15) LDURSB Wn, [<Xt|SP>{, #simm9}]

        test_entry      TEST_NO(8, 15)

        add              x2, x1, #0x20000

        ldursb           w0, [x8, #0]
        ldursb           w0, [sp, #252]
        ldursb           wzr, [x2, #-256]

        func_exit
        ret

        .end

