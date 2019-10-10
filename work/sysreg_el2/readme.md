# 作業手順

1. sysreg/sysreg-a53.csvからEL2関連のレジスタを抽出
2. 各レジスタに対してVM単位で設定するかシステム単位で設定するかを分類
3. 分類結果をsysreg_el2.csvに保存
4. sysreg_el2.csvから分類した制御単位を元にsysreg_el2.csvからsysreg_el2_vm.csv/sysreg_el2_sys.csvを作成

