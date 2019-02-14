# 機能

MAIR_ELx等、実行時のException Levelに応じたレジスタへのアクセス処理を提供するAPI群の実装

# 作業手順

1. sysreg.txtを作成
2. creelx.pyを用いてsysreg.txtからソースコードを生成
3. 生成したソースコードに同期命令をマニュアルで追加

# 制約事項

1. SCTLR_ELxの様に書き込みの前後で同期命令が必要な場合は出力結果をマニュアルで変更する必要がある
