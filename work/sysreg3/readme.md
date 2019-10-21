# 機能

EL0/EL1で利用可能なレジスタの抽出

# 作業手順

1. sysreg/sysreg-a53.csvをコピー
2. sysreg-a53.csvからPMC関連の定義を削除してsysreg-a53-no-pmc.csvを作成
3. EL1/EL0に関連するレジスタをsysreg-a53-no-pmc.csvから抽出してel0-el1.csvに保存

# 制約事項

現時点ではPerfomance Counterに関する処理は未対応とする。


