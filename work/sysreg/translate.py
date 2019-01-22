#!/usr/bin/python3
#
# (C) 2019 Hidekazu Kato
#
"""Overview:
    sysreg-a53.txtをCSV形式に変換
    変換時に不要なフィールドは削除する

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
    for ln in lines:
        if ln.find(',') < 0:
            fields = ln.split()
            if is_record(fields):
                ln = cvs_record(fields)
                print(ln)
            else:
                print(ln, end="")   # output a comment or an empty line.
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
