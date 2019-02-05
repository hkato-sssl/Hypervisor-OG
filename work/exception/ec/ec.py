#!/usr/bin/python3
#
# (C) 2019 Hidekazu Kato
#
"""Overview:
    ESR_ELx.ECに対応するメッセージ取得処理の出力

Usage:
    ec.py [-h] <file>

Options:
    -h     : help
    <file> : list file.
"""
from docopt import docopt
import os
import re
import sys

TEMPLATE_CASE = """\
    case %s:  /* 0b%s */
        "%s";
        break;
"""

import string

def readlines(file_name):
    try:
        f = open(file_name, "r")
        lines = f.readlines()
        f.close()
    except IOError as e:
        sys.exit(e)

    return lines

def translate(lines):
    for ln in lines:
        if len(ln) > 6 and ln[0:1] != "#":
            ec = ln[0:6]
            hex = format(int(ec, 2), '#04x')
            print(TEMPLATE_CASE % (hex, ec, ln[7:-1]))

def main():
    args = docopt(__doc__)
    file_name = args["<file>"]
    lines = readlines(file_name)
    translate(lines)
    return 0

if __name__ == "__main__":
    main()
