// SPDX-License-Identifier: Apache-2.0
/*
 * test/insn/01_parse_aarch64_str.S
 *
 * (C) 2020 Hidekazu Kato
 */

/* テスト内容：AArch64 STR命令の解析結果確認
 *
 * Hypervisorがエミュレーションをサポートするメモリアクセス命令を実行した
 * 際の解析結果を確認する。その為、実行内容に意味は無い。
 *
 * 1.解析対象命令
 * Hypervisorが解析を実行するSTR命令は以下の通り。
 *
 * STR (64-bit)
 * (1) STR Xn, [<Xt|SP>], #simm9
 * (2) STR Xn, [<Xt|SP>, #simm9]!
 * (3) STR Xn, [<Xt|SP>{, #pimm12}]
 * (4) STR Xn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]
 * (5) STUR Xn, [<Xt|SP>{, #simm9}]
 *
 * STR (32-bit)
 * (1) STR Wn, [<Xt|SP>], #simm9
 * (2) STR Wn, [<Xt|SP>, #simm9]!
 * (3) STR Wn, [<Xt|SP>{, #pimm12}]
 * (4) STR Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]
 * (5) STUR Wn, [<Xt|SP>{, #simm9}]
 *
 * LDR (16-bit)
 * (1) STRH Wn, [<Xt|SP>], #simm9
 * (2) STRH Wn, [<Xt|SP>, #simm9]!
 * (3) STRH Wn, [<Xt|SP>{, #pimm12}]
 * (4) STRH Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]
 * (5) STURH Wn, [<Xt|SP>{, #simm9}]
 *
 * LDR (8-bit)
 * (1) STRB Wn, [<Xt|SP>], #simm9
 * (2) STRB Wn, [<Xt|SP>, #simm9]!
 * (3) STRB Wn, [<Xt|SP>{, #pimm12}]
 * (4) STRB Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]
 * (5) STURB Wn, [<Xt|SP>{, #simm9}]
 *
 * 各パターンの(1),(2)はHypervisorが直接Instruction Codeを解析している。
 * それ以外はプロセッサが解析結果をSystem Registerに設定する。
 *
 * 2.テスト実施方針
 * テストで確認するポイントは２つ
 * (a) (1),(2)のパターンを正確に解析できているか
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

        .global         test_insn_02

        .section        ".text.test_insn_02", "ax", %progbits
        .type           test_insn_02, %function
        .balign         4
test_insn_02:
        str             lr, [sp, #-16]!

        bl              test_insn_02_str64
        bl              test_insn_02_str32
        bl              test_insn_02_str16
        bl              test_insn_02_str8

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

        .section        ".text.test_insn_02_str64", "ax", %progbits
        .type           test_insn_02_str64, %function
        .balign         4
test_insn_02_str64:
        func_entry

        // (1) STR Xn, [<Xt|SP>], #simm9

        test_entry      TEST_NO(64, 1)

        add             x2, x1, #0x20000

        str             x0, [x0], #8
        str             x0, [sp], #8
        str             xzr, [x1], #255
        str             x20, [x2], #-256

        // (2) STR Xn, [<Xt|SP>, #simm9]!

        test_entry      TEST_NO(64, 2)

        add             x2, x1, #0x20000

        str             x0, [x0, #8]!
        str             x0, [x1, #8]!
        str             xzr, [x1, #248]!
        str             x10, [sp, #248]!
        str             x20, [x2, #-256]!

        // (3) STR Xn, [<Xt|SP>{, #pimm12}]

        test_entry      TEST_NO(64, 3)

        str             xzr, [x1, #256]
        str             x0, [sp, #32760]

        // (4) STR Xn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        test_entry      TEST_NO(64, 4)

        add             x2, x1, #0x20000
        mov64           x3, 0x0000000000012340
        mov64           x4, 0xffffffff00012340
        mov64           x5, 0xffffffffffffff00
        mov64           x6, 0x00000000ffffff00

        str             x0, [sp, x3]
        str             x0, [x8, w4, uxtw]
        str             x0, [x2, x5, lsl #0]
        str             x0, [x2, x5, lsl #3]
        str             xzr, [x2, w6, sxtw]
        str             xzr, [x12, x3, sxtx]

        // (5) STUR Xn, [<Xt|SP>{, #simm9}]

        test_entry      TEST_NO(64, 5)

        add             x20, x1, #0x20000

        stur            x0, [sp, #248]
        stur            xzr, [x20, #-256]

        func_exit
        ret

        .section        ".text.test_insn_02_str32", "ax", %progbits
        .type           test_insn_02_str32, %function
        .balign         4
test_insn_02_str32:
        func_entry

        // (1) STR Wn, [<Xt|SP>], #simm9

        test_entry      TEST_NO(32, 1)

        add             x2, x1, #0x20000

        str             w0, [x0], #4
        str             w0, [sp], #4
        str             wzr, [x1], #255
        str             w20, [x2], #-256

        // (2) STR Wn, [<Xt|SP>, #simm9]!

        test_entry      TEST_NO(32, 2)

        add             x2, x1, #0x20000

        str             w0, [x0, #4]!
        str             w0, [x1, #4]!
        str             wzr, [x1, #248]!
        str             w10, [sp, #248]!
        str             w20, [x2, #-256]!

        // (3) STR Wn, [<Xt|SP>{, #pimm12}]

        test_entry      TEST_NO(32, 3)

        str             wzr, [x1, #256]
        str             w0, [sp, #16380]

        // (4) STR Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        test_entry      TEST_NO(32, 4)

        add             x2, x1, #0x20000
        mov64           x3, 0x0000000000012340
        mov64           x4, 0xffffffff00012340
        mov64           x5, 0xffffffffffffff00
        mov64           x6, 0x00000000ffffff00

        str             w0, [sp, x3]
        str             w0, [x8, w4, uxtw]
        str             w0, [x2, x5, lsl #0]
        str             w0, [x2, x5, lsl #2]
        str             wzr, [x2, w6, sxtw]
        str             wzr, [x12, x3, sxtx]

        // (5) STUR Wn, [<Xt|SP>{, #simm9}]

        test_entry      TEST_NO(32, 5)

        add             x20, x1, #0x20000

        stur            w0, [sp, #248]
        stur            wzr, [x20, #-256]

        func_exit
        ret

        .section        ".text.test_insn_02_str16", "ax", %progbits
        .type           test_insn_02_str16, %function
        .balign         4
test_insn_02_str16:
        func_entry

        // (1) STRH Wn, [<Xt|SP>], #simm9

        test_entry      TEST_NO(16, 1)

        add             x2, x1, #0x20000

        strh            w0, [x0], #2
        strh            w0, [sp], #2
        strh            wzr, [x1], #255
        strh            w20, [x2], #-256

        // (2) STRH Wn, [<Xt|SP>, #simm9]!

        test_entry      TEST_NO(16, 2)

        add             x2, x1, #0x20000

        strh            w0, [x0, #2]!
        strh            w0, [x1, #2]!
        strh            wzr, [x1, #248]!
        strh            w10, [sp, #248]!
        strh            w20, [x2, #-256]!

        // (3) STRH Wn, [<Xt|SP>{, #pimm12}]

        test_entry      TEST_NO(16, 3)

        strh            wzr, [x1, #256]
        strh            w0, [sp, #8190]

        // (4) STRH Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        test_entry      TEST_NO(16, 4)

        add             x2, x1, #0x20000
        mov64           x3, 0x0000000000012340
        mov64           x4, 0xffffffff00012340
        mov64           x5, 0xffffffffffffff00
        mov64           x6, 0x00000000ffffff00

        strh            w0, [sp, x3]
        strh            w0, [x8, w4, uxtw]
        strh            w0, [x2, x5, lsl #0]
        strh            w0, [x2, x5, lsl #1]
        strh            wzr, [x2, w6, sxtw]
        strh            wzr, [x12, x3, sxtx]

        // (5) STURH Wn, [<Xt|SP>{, #simm9}]

        test_entry      TEST_NO(16, 5)

        add             x20, x1, #0x20000

        sturh           w0, [sp, #248]
        sturh           wzr, [x20, #-256]

        func_exit
        ret

        .section        ".text.test_insn_02_str8", "ax", %progbits
        .type           test_insn_02_str8, %function
        .balign         4
test_insn_02_str8:
        func_entry

        // (1) STRB Wn, [<Xt|SP>], #simm9

        test_entry      TEST_NO(8, 1)

        add             x2, x1, #0x20000

        strb            w0, [x0], #1
        strb            w0, [sp], #1
        strb            wzr, [x1], #255
        strb            w20, [x2], #-256

        // (2) STRB Wn, [<Xt|SP>, #simm9]!

        test_entry      TEST_NO(8, 2)

        add             x2, x1, #0x20000

        strb            w0, [x0, #1]!
        strb            w0, [x1, #1]!
        strb            wzr, [x1, #248]!
        strb            w10, [sp, #248]!
        strb            w20, [x2, #-256]!

        // (3) STRB Wn, [<Xt|SP>{, #pimm12}]

        test_entry      TEST_NO(8, 3)

        strb            wzr, [x1, #256]
        strb            w20, [sp, #256]
        strb            w0, [x2, #4095]

        // (4) STRB Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        test_entry      TEST_NO(8, 4)

        add             x2, x1, #0x20000
        mov64           x3, 0x0000000000012340
        mov64           x4, 0xffffffff00012340
        mov64           x5, 0xffffffffffffff00
        mov64           x6, 0x00000000ffffff00

        strb            w0, [sp, x3]
        strb            w0, [x8, w4, uxtw]
        strb            w0, [x2, x5, lsl #0]
        strb            wzr, [x2, w6, sxtw]
        strb            wzr, [x12, x3, sxtx]

        // (5) STURB Wn, [<Xt|SP>{, #simm9}]

        test_entry      TEST_NO(8, 5)

        add             x20, x1, #0x20000

        sturb           w0, [sp, #248]
        sturb           wzr, [x20, #-256]

        func_exit
        ret

        .end

