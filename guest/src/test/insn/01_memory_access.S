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
 * HypervisorがエミュレーションをサポートするAArch64 LDR命令は以下の通り。
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
 * (4) LDRH Wn, <label>
 * (5) LDRH Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]
 * (6) LDURH Wn, [<Xt|SP>{, #simm9}]
 * (7) LDRSH Xn, [<Xt|SP>], #simm9
 * (8) LDRSH Xn, [<Xt|SP>, #simm9]!
 * (9) LDRSH Xn, [<Xt|SP>{, #pimm12}]
 * (10) LDRSH Xn, <label>
 * (11) LDRSH Xn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]
 * (12) LDURSH Xn, [<Xt|SP>{, #simm9}]
 *
 * LDR (8-bit)
 * (1) LDRB Wn, [<Xt|SP>], #simm9
 * (2) LDRB Wn, [<Xt|SP>, #simm9]!
 * (3) LDRB Wn, [<Xt|SP>{, #pimm12}]
 * (4) LDRB Wn, <label>
 * (5) LDRB Wn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]
 * (6) LDURB Wn, [<Xt|SP>{, #simm9}]
 * (7) LDRSB Xn, [<Xt|SP>], #simm9
 * (8) LDRSB Xn, [<Xt|SP>, #simm9]!
 * (9) LDRSB Xn, [<Xt|SP>{, #pimm12}]
 * (10) LDRSB Xn, <label>
 * (11) LDRSB Xn, [<Xt|SP>, <Wm|Xm>{ <extend> {<amount>}}]
 * (12) LDURSB Xn, [<Xt|SP>{, #simm9}]
 *
 * 各パターンの(1),(2),(7),(8)はHypervisorが直接Instruction Codeを
 * 解析している。
 */
        .global         test_insn_01

        .section        ".text.test_insn_01", "ax", %progbits
        .type           test_insn_01, %function
        .balign         4
test_insn_01:
        ret

        .end

