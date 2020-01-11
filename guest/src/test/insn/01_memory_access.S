/*
 * test/insn/01_memory_access.S
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

#define TRAP_REGION     0xb0000000
#define TEST_NO(s, n)   (((s) << 8) | (n))

        .global         test_insn_01

        .section        ".text.test_insn_01", "ax", %progbits
        .type           test_insn_01, %function
        .balign         4
test_insn_01:
        bl              test_insn_01_ldr64
        bl              test_insn_01_ldr32
        bl              test_insn_01_ldr16
        bl              test_insn_01_ldr8
        ret

        .section        ".text.test_insn_01_ldr64", "ax", %progbits
        .type           test_insn_01_ldr64, %function
        .balign         4
test_insn_01_ldr64:

        // (1) LDR Xn, [<Xt|SP>], #simm9

        hvc             #TEST_NO(64, 1)

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
        ldr             x30, [x1], #2
        ldr             xzr, [x1], #3

        // (2) LDR Xn, [<Xt|SP>, #simm9]!

        hvc             #TEST_NO(64, 2)

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
        ldr             x0, [x1, #255]!
        ldr             x0, [x1, #-256]!

        // (3) LDR Xn, [<Xt|SP>{, #pimm12}]

        hvc             #TEST_NO(64, 3)

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

        // (4) LDR Xn, <label>

        hvc             #TEST_NO(64, 4)

        ldr             x2, 0f
        b               1f

0:      .quad           TRAP_REGION

        .balign         4
1:
        // (5) LDR Xn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        hvc             #TEST_NO(64, 5)

        ldr             x0, =0xbbbbbbbbcccccccc
        mov             sp, x0
        ldr             x8, =0xaaaaaaaaffffffff
        ldr             x9, =0x8000000080000000

        ldr             x0, [x8, x9]
        ldr             x1, [sp, x9]
        ldr             x2, [x8, w9, uxtw]
        ldr             x3, [x8, x9, lsl #0]
        ldr             x3, [x8, x9, lsl #3]
        ldr             x4, [x8, w9, sxtw]
        ldr             xzr, [x8, x9, sxtx]

        // (6) LDUR Xn, [<Xt|SP>{, #simm9}]

        hvc             #TEST_NO(64, 5)

        ldr             x0, =0xbbbbbbbbcccccccc
        mov             sp, x0
        ldr             x8, =0xaaaaaaaaffffffff
        ldr             x9, =0x8000000080000000

        ldur             x0, [x8, #0]
        ldur             x0, [x8, #255]
        ldur             x0, [x8, #-256]

        ret

        .section        ".text.test_insn_01_ldr32", "ax", %progbits
        .type           test_insn_01_ldr32, %function
        .balign         4
test_insn_01_ldr32:
        // (1) LDR Wn, [<Xt|SP>], #simm9

        hvc             #TEST_NO(32, 1)

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
        ldr             w30, [x1], #2
        ldr             wzr, [x1], #3

        // (2) LDR Wn, [<Xt|SP>, #simm9]!

        hvc             #TEST_NO(32, 2)

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
        ldr             w0, [x1, #255]!
        ldr             w0, [x1, #-256]!

        // (3) LDR Wn, [<Xt|SP>{, #pimm12}]

        hvc             #TEST_NO(32, 3)

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

        // (4) LDR Wn, <label>

        hvc             #TEST_NO(32, 4)

        ldr             w2, 0f
        b               1f

0:      .quad           TRAP_REGION

        .balign         4
1:
        // (5) LDR Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        hvc             #TEST_NO(32, 5)

        ldr             x0, =0xbbbbbbbbcccccccc
        mov             sp, x0
        ldr             x8, =0xaaaaaaaaffffffff
        ldr             x9, =0x8000000080000000

        ldr             w0, [x8, x9]
        ldr             w1, [sp, x9]
        ldr             w2, [x8, w9, uxtw]
        ldr             w3, [x8, x9, lsl #0]
        ldr             w3, [x8, x9, lsl #2]
        ldr             w4, [x8, w9, sxtw]
        ldr             wzr, [x8, x9, sxtx]

        // (6) LDUR Wn, [<Xt|SP>{, #simm9}]

        hvc             #TEST_NO(32, 6)

        ldr             x0, =0xbbbbbbbbcccccccc
        mov             sp, x0
        ldr             x8, =0xaaaaaaaaffffffff
        ldr             x9, =0x8000000080000000

        ldur             w0, [x8, #0]
        ldur             w0, [x8, #255]
        ldur             w0, [x8, #-256]

        // (7) LDRSW Xn, [<Xt|SP>], #simm9

        hvc             #TEST_NO(32, 7)

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

        hvc             #TEST_NO(32, 8)

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
        ldrsw           x0, [x1, #255]!
        ldrsw           x0, [x1, #-256]!

        // (9) LDRSW Xn, [<Xt|SP>{, #pimm12}]

        hvc             #TEST_NO(32, 9)

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

        // (10) LDRSW Xn, <label>

        hvc             #TEST_NO(32, 10)

        ldrsw           x2, 0f
        b               1f

0:      .quad           TRAP_REGION

        .balign         4
1:
        // (11) LDRSW Xn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]

        hvc             #TEST_NO(32, 11)

        ldr             x0, =0xbbbbbbbbcccccccc
        mov             sp, x0
        ldr             x8, =0xaaaaaaaaffffffff
        ldr             x9, =0x8000000080000000

        ldrsw           x0, [x8, x9]
        ldrsw           x1, [sp, x9]
        ldrsw           x2, [x8, w9, uxtw]
        ldrsw           x3, [x8, x9, lsl #0]
        ldrsw           x3, [x8, x9, lsl #2]
        ldrsw           x4, [x8, w9, sxtw]
        ldrsw           xzr, [x8, x9, sxtx]

        // (12) LDURSW Xn, [<Xt|SP>{, #simm9}]

        hvc             #TEST_NO(32, 12)

        ldr             x0, =0xbbbbbbbbcccccccc
        mov             sp, x0
        ldr             x8, =0xaaaaaaaaffffffff
        ldr             x9, =0x8000000080000000

        ldursw          x0, [x8, #0]
        ldursw          x0, [x8, #255]
        ldursw          x0, [x8, #-256]

        ret

        .section        ".text.test_insn_01_ldr16", "ax", %progbits
        .type           test_insn_01_ldr16, %function
        .balign         4
test_insn_01_ldr16:
        // (1) LDRH Wn, [<Xt|SP>], #simm9

        hvc             #TEST_NO(16, 1)

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
        ldrh            w30, [x1], #2
        ldrh            wzr, [x1], #3

        // (2) LDRH Wn, [<Xt|SP>, #simm9]!

        hvc             #TEST_NO(16, 2)

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
        ldrh            w0, [x1, #255]!
        ldrh            w0, [x1, #-256]!

        // (3) LDRH Wn, [<Xt|SP>{, #pimm12}]

        hvc             #TEST_NO(16, 3)

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

        hvc             #TEST_NO(16, 4)

        ldr             x0, =0xbbbbbbbbcccccccc
        mov             sp, x0
        ldr             x8, =0xaaaaaaaaffffffff
        ldr             x9, =0x8000000080000000

        ldrh            w0, [x8, x9]
        ldrh            w1, [sp, x9]
        ldrh            w2, [x8, w9, uxtw]
        ldrh            w3, [x8, x9, lsl #0]
        ldrh            w3, [x8, x9, lsl #1]
        ldrh            w4, [x8, w9, sxtw]
        ldrh            wzr, [x8, x9, sxtx]

        // (5) LDURH Wn, [<Xt|SP>{, #simm9}]

        hvc             #TEST_NO(16, 5)

        ldr             x0, =0xbbbbbbbbcccccccc
        mov             sp, x0
        ldr             x8, =0xaaaaaaaaffffffff
        ldr             x9, =0x8000000080000000

        ldurh           w0, [x8, #0]
        ldurh           w0, [x8, #255]
        ldurh           w0, [x8, #-256]

        // (6) LDRSH Xn, [<Xt|SP>], #simm9

        hvc             #TEST_NO(16, 6)

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

        hvc             #TEST_NO(16, 7)

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
        ldrsh           x0, [x1, #255]!
        ldrsh           x0, [x1, #-256]!

        // (8) LDRSH Xn, [<Xt|SP>{, #pimm12}]

        hvc             #TEST_NO(16, 8)

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

        hvc             #TEST_NO(16, 9)

        ldr             x0, =0xbbbbbbbbcccccccc
        mov             sp, x0
        ldr             x8, =0xaaaaaaaaffffffff
        ldr             x9, =0x8000000080000000

        ldrsh           x0, [x8, x9]
        ldrsh           x1, [sp, x9]
        ldrsh           x2, [x8, w9, uxtw]
        ldrsh           x3, [x8, x9, lsl #0]
        ldrsh           x3, [x8, x9, lsl #1]
        ldrsh           x4, [x8, w9, sxtw]
        ldrsh           xzr, [x8, x9, sxtx]

        // (10) LDURSH Xn, [<Xt|SP>{, #simm9}]

        hvc             #TEST_NO(16, 10)

        ldr             x0, =0xbbbbbbbbcccccccc
        mov             sp, x0
        ldr             x8, =0xaaaaaaaaffffffff
        ldr             x9, =0x8000000080000000

        ldursh          x0, [x8, #0]
        ldursh          x0, [x8, #255]
        ldursh          x0, [x8, #-256]

        ret

        .section        ".text.test_insn_01_ldr8", "ax", %progbits
        .type           test_insn_01_ldr8, %function
        .balign         4
test_insn_01_ldr8:
        // (1) LDRB Wn, [<Xt|SP>], #simm9

        hvc             #TEST_NO(8, 1)

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
        ldrb            w30, [x1], #2
        ldrb            wzr, [x1], #3

        // (2) LDRB Wn, [<Xt|SP>, #simm9]!

        hvc             #TEST_NO(8, 2)

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
        ldrb            w0, [x1, #-256]!

        // (3) LDRB Wn, [<Xt|SP>{, #pimm12}]

        hvc             #TEST_NO(8, 3)

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

        hvc             #TEST_NO(8, 4)

        ldr             x0, =0xbbbbbbbbcccccccc
        mov             sp, x0
        ldr             x8, =0xaaaaaaaaffffffff
        ldr             x9, =0x8000000080000000

        ldrb            w0, [x8, x9]
        ldrb            w1, [sp, x9]
        ldrb            w2, [x8, w9, uxtw]
        ldrb            w3, [x8, x9, lsl #0]
        ldrb            w4, [x8, w9, sxtw]
        ldrb            wzr, [x8, x9, sxtx]

        // (5) LDURB Wn, [<Xt|SP>{, #simm9}]

        hvc             #TEST_NO(8, 5)

        ldr             x0, =0xbbbbbbbbcccccccc
        mov             sp, x0
        ldr             x8, =0xaaaaaaaaffffffff
        ldr             x9, =0x8000000080000000

        ldurb           w0, [x8, #0]
        ldurb           w0, [x8, #255]
        ldurb           w0, [x8, #-256]

        // (6) LDRSB Xn, [<Xt|SP>], #simm9

        hvc             #TEST_NO(8, 6)

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

        hvc             #TEST_NO(8, 7)

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
        ldrsb           x0, [x1, #-256]!

        // (8) LDRSB Xn, [<Xt|SP>{, #pimm12}]

        hvc             #TEST_NO(8, 8)

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

        hvc             #TEST_NO(8, 9)

        ldr             x0, =0xbbbbbbbbcccccccc
        mov             sp, x0
        ldr             x8, =0xaaaaaaaaffffffff
        ldr             x9, =0x8000000080000000

        ldrsb           x0, [x8, x9]
        ldrsb           x1, [sp, x9]
        ldrsb           x2, [x8, w9, uxtw]
        ldrsb           x3, [x8, x9, lsl #0]
        ldrsb           x4, [x8, w9, sxtw]
        ldrsb           xzr, [x8, x9, sxtx]

        // (10) LDURSB Xn, [<Xt|SP>{, #simm9}]

        hvc             #TEST_NO(8, 10)

        ldr             x0, =0xbbbbbbbbcccccccc
        mov             sp, x0
        ldr             x8, =0xaaaaaaaaffffffff
        ldr             x9, =0x8000000080000000

        ldursb          x0, [x8, #0]
        ldursb          x0, [x8, #255]
        ldursb          x0, [x8, #-256]

        ret

        .end

