#!/usr/bin/python3
#
# (C) 2019 Hidekazu Kato
#
"""Overview:
    EL2.csvからvcpu用マクロ定義を生成
    A53フィールドが〇のレジスタを生成対象とする

Usage:
    vcpu-el2.py [-h] <file>

Options:
    -h       : help
    <file>   : csv file name.
"""
from docopt import docopt
import os
import re
import sys
import string

base_name = 'VCPU_SYS_EL2_BASE';
next_base_name = 'VCPU_FPU_BASE';

def readlines(file_name):
    try:
        f = open(file_name, "r")
        lines = f.readlines()
    except IOError as e:
        sys.exit(e)
    finally:
        f.close()

    return lines

def translate(lines):
    global base_name;
    offset = 0;
    for ln in lines:
        fields = ln.split(',')
        if fields[4] == '〇':
            def_name = '#define VCPU_' + fields[0] + ' ' * (24 - 5 - len(fields[0]))
            if offset == 0:
                def_val = base_name
            else:
                def_val = '(' + base_name + " + " + str(offset) + ')'
            comment = (' ' * (24 - len(def_val))) + '// ' + fields[3]
            print(def_name, def_val, comment)
            offset += 1

    def_name = '#define ' + next_base_name + ' ' * (24 - len(next_base_name))
    def_val = '(' + base_name + " + " + str(offset) + ')'
    print(def_name, def_val)

def main():
    args = docopt(__doc__)
    file_name = args["<file>"]
    lines = readlines(file_name)

    # remove the first element
    del lines[0]

    translate(lines)
    return 0

if __name__ == "__main__":
    main()
