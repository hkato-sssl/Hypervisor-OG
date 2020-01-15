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
 * hvc #0はテスト完了とする。
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

        ldr             x0, [x1], #8
        ldr             x7, [x1], #8
        ldr             x30, [x1], #8
        ldr             xzr, [x1], #8

        ldr             x0, [x9], #8
        ldr             x7, [x8], #8
        ldr             x30, [x10], #8
        ldr             xzr, [SP], #8

        ldr             x0, [x1], #0
        ldr             x7, [x1], #1
        ldr             x30, [x1], #128
        ldr             xzr, [x1], #255
        ldr             xzr, [x1], #-256

        // (2) LDR Xn, [<Xt|SP>, #simm9]!

        test_entry      TEST_NO(64, 2)

        add             x2, x1, #0x1000

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

        ldr             x9, =0x00012340
        ldr             x10, =0xffffffff00012340
        mov             x11, #32
        add             x12, x8, #0x20000

        ldr             x0, [x8, x9]
        ldr             x1, [sp, x9]
        ldr             x2, [x8, w10, uxtw]
        ldr             x3, [x8, x11, lsl #0]
        ldr             x3, [x8, x11, lsl #3]
        ldr             x4, [x8, w10, sxtw]
        mov             x10, #-8
        ldr             xzr, [x12, x10, sxtx]

        // (6) LDUR Xn, [<Xt|SP>{, #simm9}]

        test_entry      TEST_NO(64, 5)

        add             x9, x8, #0x10000

        ldur             x0, [x8, #0]
        ldur             x0, [x8, #248]
        ldur             x0, [x9, #-256]

        func_exit
        ret

        .section        ".text.test_insn_01_ldr32", "ax", %progbits
        .type           test_insn_01_ldr32, %function
        .balign         4
test_insn_01_ldr32:
        func_entry

        // (1) LDR Wn, [<Xt|SP>], #simm9

        test_entry      TEST_NO(32, 1)

        ldr             w0, [x1], #8
        ldr             w7, [x1], #8
        ldr             w30, [x1], #8
        ldr             wzr, [x1], #8

        ldr             w0, [x9], #8
        ldr             w7, [x8], #8
        ldr             w30, [x10], #8
        ldr             wzr, [SP], #8

        ldr             w0, [x1], #0
        ldr             w7, [x1], #1
        ldr             wzr, [x1], #128
        ldr             wzr, [x1], #255
        ldr             wzr, [x1], #-256

        // (2) LDR Wn, [<Xt|SP>, #simm9]!

        test_entry      TEST_NO(32, 2)

        add             x2, x1, #0x10000

        ldr             w0, [x1, #8]!
        ldr             w7, [x1, #8]!
        ldr             w30, [x1, #8]!
        ldr             wzr, [x1, #8]!

        ldr             w0, [x0, #8]!
        ldr             w0, [x9, #8]!
        ldr             w0, [x30, #8]!
        ldr             w0, [sp, #8]!

        ldr             w0, [x1, #0]!
        ldr             w0, [x1, #128]!
        ldr             w0, [x1, #252]!
        ldr             w0, [x2, #-256]!

        // (3) LDR Wn, [<Xt|SP>{, #pimm12}]

        test_entry      TEST_NO(32, 3)

        ldr             w0, [x1, #8]
        ldr             w7, [x1, #8]
        ldr             w30, [x1, #8]
        ldr             wzr, [x1, #8]

        ldr             w0, [x0, #8]
        ldr             w0, [x7, #8]
        ldr             w0, [x30, #8]
        ldr             w0, [sp, #8]

        ldr             w0, [x0, #0]
        ldr             w0, [x0, #256]
        ldr             w0, [x0, #16380]

        // (5) LDR Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        test_entry      TEST_NO(32, 5)

        ldr             x9, =0x00012340
        ldr             x10, =0xfffffffffffffff8
        mov             x11, #256
        ldr             x12, =0xfffffff0
        mov             x13, #0x100000000
        add             x13, x8, x13

        ldr             w0, [x8, x9]
        ldr             w1, [sp, x9]
        ldr             w2, [x8, w11, uxtw]
        ldr             w3, [x8, x11, lsl #0]
        ldr             w3, [x8, x11, lsl #2]
        ldr             w4, [x13, w12, sxtw]
        ldr             wzr, [x13, x10, sxtx]

        // (6) LDUR Wn, [<Xt|SP>{, #simm9}]

        test_entry      TEST_NO(32, 6)

        add              x8, x8, #0x1000
        ldur             w0, [x8, #0]
        ldur             w0, [x8, #252]
        ldur             w0, [x8, #-256]

        // (7) LDRSW Xn, [<Xt|SP>], #simm9

        test_entry      TEST_NO(32, 7)

        ldrsw           x0, [x1], #8
        ldrsw           x7, [x1], #8
        ldrsw           x30, [x1], #8
        ldrsw           xzr, [x1], #8

        ldrsw           x0, [x9], #8
        ldrsw           x7, [x8], #8
        ldrsw           x30, [x10], #8
        ldrsw           xzr, [SP], #8

        ldrsw           x0, [x1], #0
        ldrsw           x7, [x1], #1
        ldrsw           x30, [x1], #2
        ldrsw           xzr, [x1], #3

        // (8) LDRSW Xn, [<Xt|SP>, #simm9]!

        test_entry      TEST_NO(32, 8)

        add             x2, x1, #0x10000

        ldrsw           x0, [x1, #8]!
        ldrsw           x7, [x1, #8]!
        ldrsw           x30, [x1, #8]!
        ldrsw           xzr, [x1, #8]!

        ldrsw           x0, [x0, #8]!
        ldrsw           x0, [x9, #8]!
        ldrsw           x0, [x30, #8]!
        ldrsw           x0, [sp, #8]!

        ldrsw           x0, [x1, #0]!
        ldrsw           x0, [x1, #128]!
        ldrsw           x0, [x1, #252]!
        ldrsw           x0, [x2, #-256]!

        // (9) LDRSW Xn, [<Xt|SP>{, #pimm12}]

        test_entry      TEST_NO(32, 9)

        ldrsw           x0, [x1, #8]
        ldrsw           x7, [x1, #8]
        ldrsw           x30, [x1, #8]
        ldrsw           xzr, [x1, #8]

        ldrsw           x0, [x0, #8]
        ldrsw           x0, [x7, #8]
        ldrsw           x0, [x30, #8]
        ldrsw           x0, [sp, #8]

        ldrsw           x0, [x0, #0]
        ldrsw           x0, [x0, #256]
        ldrsw           x0, [x0, #16380]

        // (11) LDRSW Xn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        test_entry      TEST_NO(32, 11)

        ldr             x9, =0x00012340
        ldr             x10, =0xffffffff00012340
        mov             x11, #16
        ldr             x12, =0xa0000000

        ldrsw           x0, [x8, x9]
        ldrsw           x1, [sp, x9]
        ldrsw           x2, [x8, w10, uxtw]
        ldrsw           x3, [x8, x11, lsl #0]
        ldrsw           x3, [x8, x11, lsl #2]
        ldrsw           x4, [x8, w10, sxtw]
        ldrsw           xzr, [x8, x12, sxtx]

        // (12) LDURSW Xn, [<Xt|SP>{, #simm9}]

        test_entry      TEST_NO(32, 12)

        add             x9, x8, #0x10000

        ldursw          x0, [x8, #0]
        ldursw          x0, [x8, #252]
        ldursw          x0, [x9, #-256]

        func_exit
        ret

        .section        ".text.test_insn_01_ldr16", "ax", %progbits
        .type           test_insn_01_ldr16, %function
        .balign         4
test_insn_01_ldr16:
        func_entry

        // (1) LDRH Wn, [<Xt|SP>], #simm9

        test_entry      TEST_NO(16, 1)

        ldrh            w0, [x1], #8
        ldrh            w7, [x1], #8
        ldrh            w30, [x1], #8
        ldrh            wzr, [x1], #8

        ldrh            w0, [x9], #8
        ldrh            w7, [x8], #8
        ldrh            w30, [x10], #8
        ldrh            wzr, [SP], #8

        ldrh            w0, [x1], #0
        ldrh            w7, [x1], #1
        ldrh            w30, [x1], #128
        ldrh            wzr, [x1], #255
        ldrh            wzr, [x1], #-256

        // (2) LDRH Wn, [<Xt|SP>, #simm9]!

        test_entry      TEST_NO(16, 2)

        add             x2, x1, #0x10000

        ldrh            w0, [x1, #8]!
        ldrh            w7, [x1, #8]!
        ldrh            w30, [x1, #8]!
        ldrh            wzr, [x1, #8]!

        ldrh            w0, [x0, #8]!
        ldrh            w0, [x9, #8]!
        ldrh            w0, [x30, #8]!
        ldrh            w0, [sp, #8]!

        ldrh            w0, [x1, #0]!
        ldrh            w0, [x1, #128]!
        ldrh            w0, [x1, #254]!
        ldrh            w0, [x2, #-256]!

        // (3) LDRH Wn, [<Xt|SP>{, #pimm12}]

        test_entry      TEST_NO(16, 3)

        ldrh            w0, [x1, #8]
        ldrh            w7, [x1, #8]
        ldrh            w30, [x1, #8]
        ldrh            wzr, [x1, #8]

        ldrh            w0, [x0, #8]
        ldrh            w0, [x7, #8]
        ldrh            w0, [x30, #8]
        ldrh            w0, [sp, #8]

        ldrh            w0, [x0, #0]
        ldrh            w0, [x0, #256]
        ldrh            w0, [x0, #8190]

        // (4) LDRH Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        test_entry      TEST_NO(16, 4)

        mov             x9, #0xff0
        add             x10, x8, #0x10000
        ldr             x11, =0xfffffff0

        ldrh            w0, [x8, x9]
        ldrh            w1, [sp, x9]
        ldrh            w2, [x8, w9, uxtw]
        ldrh            w3, [x8, x9, lsl #0]
        ldrh            w3, [x8, x9, lsl #1]
        ldrh            w4, [x10, w11, sxtw]
        ldrh            wzr, [x8, x11, sxtx]

        // (5) LDURH Wn, [<Xt|SP>{, #simm9}]

        test_entry      TEST_NO(16, 5)

        add             x9, x8, #0x100

        ldurh           w0, [x8, #0]
        ldurh           w0, [x8, #254]
        ldurh           w0, [x9, #-256]

        // (6) LDRSH Xn, [<Xt|SP>], #simm9

        test_entry      TEST_NO(16, 6)

        ldrsh           x0, [x1], #8
        ldrsh           x7, [x1], #8
        ldrsh           x30, [x1], #8
        ldrsh           xzr, [x1], #8

        ldrsh           x0, [x9], #8
        ldrsh           x7, [x8], #8
        ldrsh           x30, [x10], #8
        ldrsh           xzr, [SP], #8

        ldrsh           x0, [x1], #0
        ldrsh           x7, [x1], #1
        ldrsh           x30, [x1], #2
        ldrsh           xzr, [x1], #3

        // (7) LDRSH Xn, [<Xt|SP>, #simm9]!

        test_entry      TEST_NO(16, 7)

        add             x2, x1, #0x10000

        ldrsh           x0, [x1, #8]!
        ldrsh           x7, [x1, #8]!
        ldrsh           x30, [x1, #8]!
        ldrsh           xzr, [x1, #8]!

        ldrsh           x0, [x0, #8]!
        ldrsh           x0, [x9, #8]!
        ldrsh           x0, [x30, #8]!
        ldrsh           x0, [sp, #8]!

        ldrsh           x0, [x1, #0]!
        ldrsh           x0, [x1, #128]!
        ldrsh           x0, [x1, #254]!
        ldrsh           x0, [x2, #-256]!

        // (8) LDRSH Xn, [<Xt|SP>{, #pimm12}]

        test_entry      TEST_NO(16, 8)

        ldrsh           x0, [x1, #8]
        ldrsh           x7, [x1, #8]
        ldrsh           x30, [x1, #8]
        ldrsh           xzr, [x1, #8]

        ldrsh           x0, [x0, #8]
        ldrsh           x0, [x7, #8]
        ldrsh           x0, [x30, #8]
        ldrsh           x0, [sp, #8]

        ldrsh           x0, [x0, #0]
        ldrsh           x0, [x0, #256]
        ldrsh           x0, [x0, #8190]

        // (9) LDRSH Xn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        test_entry      TEST_NO(16, 9)

        mov             x9, #0xfffffff0
        mov             x10, #0x100000000
        add             x10, x10, x8
        mov             x11, #32

        ldrsh           x0, [x8, x9]
        ldrsh           x1, [sp, x9]
        ldrsh           x2, [x8, w9, uxtw]
        ldrsh           x3, [x8, x9, lsl #0]
        ldrsh           x3, [x8, x11, lsl #1]
        ldrsh           x4, [x10, w9, sxtw]
        ldrsh           xzr, [x8, x9, sxtx]

        // (10) LDURSH Xn, [<Xt|SP>{, #simm9}]

        test_entry      TEST_NO(16, 10)

        add             x9, x8, #0x10000

        ldursh          x0, [x8, #0]
        ldursh          x0, [x8, #254]
        ldursh          x0, [x9, #-256]

        func_exit
        ret

        .section        ".text.test_insn_01_ldr8", "ax", %progbits
        .type           test_insn_01_ldr8, %function
        .balign         4
test_insn_01_ldr8:
        func_entry

        // (1) LDRB Wn, [<Xt|SP>], #simm9

        test_entry      TEST_NO(8, 1)

        ldrb            w0, [x1], #8
        ldrb            w7, [x1], #8
        ldrb            w30, [x1], #8
        ldrb            wzr, [x1], #8

        ldrb            w0, [x9], #8
        ldrb            w7, [x8], #8
        ldrb            w30, [x10], #8
        ldrb            wzr, [SP], #8

        ldrb            w0, [x1], #0
        ldrb            w7, [x1], #1
        ldrb            w30, [x1], #128
        ldrb            wzr, [x1], #255
        ldrb            wzr, [x1], #-256

        // (2) LDRB Wn, [<Xt|SP>, #simm9]!

        test_entry      TEST_NO(8, 2)

        add             x2, x1, #0x1000

        ldrb            w0, [x1, #8]!
        ldrb            w7, [x1, #8]!
        ldrb            w30, [x1, #8]!
        ldrb            wzr, [x1, #8]!

        ldrb            w0, [x0, #8]!
        ldrb            w0, [x9, #8]!
        ldrb            w0, [x30, #8]!
        ldrb            w0, [sp, #8]!

        ldrb            w0, [x1, #0]!
        ldrb            w0, [x1, #128]!
        ldrb            w0, [x1, #255]!
        ldrb            w0, [x2, #-256]!

        // (3) LDRB Wn, [<Xt|SP>{, #pimm12}]

        test_entry      TEST_NO(8, 3)

        ldrb            w0, [x1, #8]
        ldrb            w7, [x1, #8]
        ldrb            w30, [x1, #8]
        ldrb            wzr, [x1, #8]

        ldrb            w0, [x0, #8]
        ldrb            w0, [x7, #8]
        ldrb            w0, [x30, #8]
        ldrb            w0, [sp, #8]

        ldrb            w0, [x0, #0]
        ldrb            w0, [x0, #256]
        ldrb            w0, [x0, #4095]

        // (4) LDRB Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        test_entry      TEST_NO(8, 4)

        mov             x9, #0x1000
        mov             x10, #0xfffffffffffffff0
        mov             x11, #33

        ldrb            w0, [x8, x9]
        ldrb            w1, [sp, x9]
        ldrb            w2, [x8, w10, uxtw]
        ldrb            w3, [x8, x11, lsl #0]
        ldrb            w4, [x8, w9, sxtw]
        ldrb            wzr, [x8, x9, sxtx]

        // (5) LDURB Wn, [<Xt|SP>{, #simm9}]

        test_entry      TEST_NO(8, 5)

        add             x9, x8, #0x2000

        ldurb           w0, [x8, #0]
        ldurb           w0, [x8, #255]
        ldurb           w0, [x9, #-256]

        // (6) LDRSB Xn, [<Xt|SP>], #simm9

        test_entry      TEST_NO(8, 6)

        ldrsb           x0, [x1], #8
        ldrsb           x7, [x1], #8
        ldrsb           x30, [x1], #8
        ldrsb           xzr, [x1], #8

        ldrsb           x0, [x9], #8
        ldrsb           x7, [x8], #8
        ldrsb           x30, [x10], #8
        ldrsb           xzr, [SP], #8

        ldrsb           x0, [x1], #0
        ldrsb           x7, [x1], #1
        ldrsb           x30, [x1], #2
        ldrsb           xzr, [x1], #3

        // (7) LDRSB Xn, [<Xt|SP>, #simm9]!

        test_entry      TEST_NO(8, 7)

        add             x2, x1, #0x1000

        ldrsb           x0, [x1, #8]!
        ldrsb           x7, [x1, #8]!
        ldrsb           x30, [x1, #8]!
        ldrsb           xzr, [x1, #8]!

        ldrsb           x0, [x0, #8]!
        ldrsb           x0, [x9, #8]!
        ldrsb           x0, [x30, #8]!
        ldrsb           x0, [sp, #8]!

        ldrsb           x0, [x1, #0]!
        ldrsb           x0, [x1, #128]!
        ldrsb           x0, [x1, #255]!
        ldrsb           x0, [x2, #-256]!

        // (8) LDRSB Xn, [<Xt|SP>{, #pimm12}]

        test_entry      TEST_NO(8, 8)

        ldrsb           x0, [x1, #8]
        ldrsb           x7, [x1, #8]
        ldrsb           x30, [x1, #8]
        ldrsb           xzr, [x1, #8]

        ldrsb           x0, [x0, #8]
        ldrsb           x0, [x7, #8]
        ldrsb           x0, [x30, #8]
        ldrsb           x0, [sp, #8]

        ldrsb           x0, [x0, #0]
        ldrsb           x0, [x0, #256]
        ldrsb           x0, [x0, #4095]

        // (9) LDRSB Xn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        test_entry      TEST_NO(8, 9)

        mov             x9, #0x1000
        mov             x10, #0xffffffffffffffff
        add             x11, x8, #0x20000

        ldrsb           x0, [x8, x9]
        ldrsb           x1, [sp, x9]
        ldrsb           x2, [x8, w9, uxtw]
        ldrsb           x3, [x8, x9, lsl #0]
        ldrsb           x4, [x11, w10, sxtw]
        ldrsb           xzr, [x11, x10, sxtx]

        // (10) LDURSB Xn, [<Xt|SP>{, #simm9}]

        test_entry      TEST_NO(8, 10)

        mov             x9, #0xffff
        add             x9, x9, x8

        ldursb          x0, [x8, #0]
        ldursb          x0, [x8, #255]
        ldursb          x0, [x9, #-256]

        func_exit
        ret

        .end

