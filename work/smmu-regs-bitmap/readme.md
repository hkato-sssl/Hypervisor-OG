# 目的

SMMU用レジスタビットマップ定義生成

# 作業手順

1. Reference ManualからCopy&PasteでCSVファイルを作成
   例）SMMU_CB_SCTLR.csv
2. regs-bitmap.pyでCSVからマクロ定義を生成
   マクロのPrefixは入力ファイル名となる

# 制約事項

* CSVの中身に関しては厳密なチェックは行っていない
* 基本的にエラーとならないCSVを渡す必要がある
