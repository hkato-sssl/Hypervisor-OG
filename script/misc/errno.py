#!/usr/bin/python3
#
# (C) 2018 Hidekazu Kato
#
"""Overview:
    Generate definition list for 'errno.h'.

Usage:
    errno.py [-h] <file>

Options:
    -h     : help
    <file> : CSV file name.
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
        f.close()
    except IOError as e:
        sys.exit(e)

    return lines

def translate(lines):
    words = []
    no = 1
    for ln in lines:
        fields = ln.rstrip().split(',', 1)
        match = re.search(r'\[[^\s]+\]', fields[1])
        if match:
            wd = match.group(0)[1:-1]
            if wd in words:
                print("#define {0}\t\t{1}\t/* {2} */".format(fields[0], wd, fields[1]))
            else:
                print("#define {0}\t\t{1}\t/* {2} */".format(fields[0], no, fields[1]))
                no += 1
        else:
            print("#define {0}\t\t{1}\t/* {2} */".format(fields[0], no, fields[1]))
            no += 1
        words.append(fields[0])

def main():
    args = docopt(__doc__)
    file_name = args["<file>"]
    lines = readlines(file_name)
    translate(lines)
    return 0

if __name__ == "__main__":
    main()
