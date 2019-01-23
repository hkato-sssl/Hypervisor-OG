#!/usr/bin/python3
#
# (C) 2019 Hidekazu Kato
#
"""Overview:
    sysreg-a53.txtをCSV形式に変換。
    変換時に不要なフィールドは削除する。
    重複した定義が存在する場合、最初の定義以外は無視する。

Usage:
    translate.py [-h] <file>

Options:
    -h     : help
    <file> : text file name.
"""
from docopt import docopt
import os
import re
import sys
import string

def readlines(file_name):
    try:
        f = open(file_name, "r")
        lines = f.readlines()
    except IOError as e:
        sys.exit(e)
    finally:
        f.close()

    return lines

def is_record(fields):
    if len(fields) == 0:
        result = False
    else:
        result = re.match(r'[^#]', fields[0])

    return result

def cvs_record(fields):
    reg = fields[0]
    access = fields[1]
    width = fields[3]
    if len(fields) > 4:
        comment = " ".join(fields[4:])
        cvs = reg + "," + access + "," + width + "," + comment
    else:
        cvs = reg + "," + access + "," + width 

    return cvs

def translate(lines):
    registers = []
    comment = ""
    for ln in lines:
        if ln.find(',') < 0:
            fields = ln.split()
            if is_record(fields):
                if fields[0] not in registers:
                    registers.append(fields[0])
                    if comment != "":
                        print('\n' + comment)
                        comment = ""
                    ln = cvs_record(fields)
                    print(ln)
            elif len(fields) > 0 and fields[0] == "#":
                comment = ln
        else:
            raise Exception("a line has an illegal character ','")

def main():
    args = docopt(__doc__)
    file_name = args["<file>"]
    lines = readlines(file_name)
    translate(lines)
    return 0

if __name__ == "__main__":
    main()
