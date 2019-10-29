# 目的

vpc_ctx_load_system_register()/vpc_ctx_save_system_register()の自動生成

# 作業手順

1. docs/vpc/配下の各エクセルシートからEL0-EL1.csv/EL2.csvを生成
2. vpc-ctx-sys.pyでCSVからソースファイルを生成

# 制約事項

* 変換に利用するCSVファイルはスクリプト中のFILESに記述する
