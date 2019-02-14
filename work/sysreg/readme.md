# 作業手順

1. Cortex-A53 TRM内のSystem Registerの記述をコピペしてsysreg-a53.txtを作成
2. 手書きで「Part of PSTATE」を追記
3. translate.pyを用いてsysreg-a53.txtをsysreg-a53.csvに変換
4. validation.pyでsysreg-a53.csvの内容を検証
5. create.pyでsysreg-a53.csvからsysreg-a53.cを生成

# 制約事項

1. 同期処理に未対応<br>
SCTLR_ELxの様に書き込みを行う際に同期処理が必要なレジスタが存在するが、現状は同期処理の出力には対応していない。同期処理が必要な場合は生成コードに対してマニュアルで書き換える必要がある。

