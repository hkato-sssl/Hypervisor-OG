#!/usr/bin/python3
#
# (C) 2019 Hidekazu Kato
#
"""Overview:
    CSVファイルからregister array定義マクロを生成
    変換元となるCSVファイル名はスクリプト内部に記述されている
    A53フィールドが〇のレジスタを生成対象とする

Usage:
    vcpu-regs.py [-h]

Options:
    -h       : help
"""
from docopt import docopt
import os
import re
import sys
import string

PREFIX = 'VPC_'
DEF_NAME_WIDTH = 24
DEF_VALUE_WIDTH = 8
FILES = [ 'SPR', 'EL0-EL1', 'EL2', 'FP-SIMD']

offset = 0;

def out_desc(name, comment):
    global PREFIX
    global DEF_NAME_WIDTH
    global DEF_VALUE_WIDTH
    global offset

    spc1 = ' ' * (DEF_NAME_WIDTH - len(name))
    value = str(offset)
    if len(comment) > 0:
        spc2 = ' ' * (DEF_VALUE_WIDTH - len(value))
        print('#define ' + name + spc1 + value + spc2 + '// ' + comment)
    else:
        print('#define ' + name + spc1 + value)
    offset += 1

def readlines(file_name):
    try:
        f = open(file_name, "r")
        lines = f.readlines()
    except IOError as e:
        sys.exit(e)
    finally:
        f.close()

    return lines

def create_desc(lines):
    for ln in lines:
        fields = ln.split(',')
        if fields[4] == '〇':
            out_desc(PREFIX + fields[0], fields[3])

def create_gpr():
    for n in range(31):
        out_desc(PREFIX + 'X' + str(n), '')

def create_fpu():
    global offset
    for n in range(32):
        out_desc(PREFIX + 'Q' + str(n), '')
        offset += 1

def main():
    global files

    create_gpr()
    for fn in FILES:
        lines = readlines(fn + '.csv')
        create_desc(lines)
    create_fpu()
    out_desc('NR_' + PREFIX + 'REGS', '')

    return 0

if __name__ == "__main__":
    main()
