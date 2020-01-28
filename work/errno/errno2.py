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
    values = {}
    no = 10
    for ln in lines:
        fields = ln.rstrip().split(',', 1)
        match = re.search(r'\[[^\s]+\]', fields[1])
        if match:
            wd = match.group(0)[1:-1]
            if wd in words:
                print("#define {0}\t\tsystem_errno({1}, __FILE__, __LINE__)".format(fields[0], values[wd]))
            else:
                values[fields[0]] = no
                print("#define {0}\t\tsystem_errno({1}, __FILE__, __LINE__)".format(fields[0], no))
                no += 1
        else:
            values[fields[0]] = no
            print("#define {0}\t\tsystem_errno({1}, __FILE__, __LINE__)".format(fields[0], no))
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
