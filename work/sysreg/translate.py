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

def is_record(line):
    if len(line) == 0:
        result = false
    else:
        result = re.match(r'[^#\r\n]', line)

    return result

def cvs_record(line):
    fields = line.split()
    reg = fields.pop(0)
    access = fields.pop(0)
    del fields[0:2]
    if len(fields) > 0:
        comment = " ".join(fields)
        cvs = "{0},{1},{2}".format(reg, access, comment)
    else:
        cvs = "{0},{1}".format(reg, access)

    return cvs

def translate(lines):
    for ln in lines:
        if is_record(ln):
            ln = cvs_record(ln)
            print(ln)
        else:
            print(ln, end="")   # output a comment or an empty line.

def main():
    args = docopt(__doc__)
    file_name = args["<file>"]
    lines = readlines(file_name)
    translate(lines)
    return 0

if __name__ == "__main__":
    main()
