#!/usr/bin/python3
#
# (C) 2019 Hidekazu Kato
#
"""Overview:
    sysreg-a53.txtの検証用スクリプト

Usage:
    validate.py [-h] <file>

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

def validate(lines):
    no = 0
    for ln in lines:
        no += 1
        if is_record(ln):
            fields = ln.split()
            if len(fields) < 4:
                print("#%d: Illegal line." % no)
                break
            elif fields[3] != "32" and fields[3] != "64":
                print("#%d: Illegal field[3]." % no)
                break

def main():
    args = docopt(__doc__)
    file_name = args["<file>"]
    lines = readlines(file_name)
    validate(lines)
    return 0

if __name__ == "__main__":
    main()
