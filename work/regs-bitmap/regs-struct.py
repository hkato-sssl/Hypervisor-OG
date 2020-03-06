#!/usr/bin/python3
#
# (C) 2019 Hidekazu Kato
#
"""Overview:
    CSVファイルからstruct定義文の雛形を生成

Usage:
    regs-struct.py [-h] <file>

Options:
    -h       : help
"""
from docopt import docopt
import os
import re
import sys
import string

def out_def_member(name, width):
    print('    uint32_t    {}:{};'.format(name.lower().strip(' '), width))

def pick_up(ln):
    ln = ln.split(',')
    l = len(ln)

    ln[0] = re.sub(r'\[\d:0\]', '', ln[0])
    
    if l == 1:
        ret = [ln[0]]
    elif l > 1:
        m1 = re.search(r'\[(\d+)', ln[1])
        m2 = re.search(r':(\d+)', ln[1])
        if bool(m2):
            ret = [ln[0], m1.group(1), m2.group(1)]
        else:
            ret = [ln[0], m1.group(1)]

    return ret

def out_desc(file, lines):
    pos = file.find('.')
    if pos >= 0:
        name = file[0:pos]
    else:
        name = file

    print('struct {} {{'.format(name.lower()))

    for ln in lines:
        fields = pick_up(ln)
        fl = len(fields)
        if fl == 2:
            out_def_member(fields[0], 1);
        elif fl > 2:
            out_def_member(fields[0], ((int(fields[1]) - int(fields[2])) + 1))

    print('};')

def readlines(file_name):
    try:
        f = open(file_name, "r")
        lines = f.readlines()
    except IOError as e:
        sys.exit(e)
    finally:
        f.close()

    return lines

def main(args):
    lines = readlines(args['<file>'])
    out_desc(args['<file>'], lines)

    return 0

if __name__ == "__main__":
    args = docopt(__doc__)
    main(args)
