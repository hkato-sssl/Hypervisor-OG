#!/usr/bin/python3
#
# (C) 2019 Hidekazu Kato
#
"""Overview:
    CSVファイルからregister bitmap定義マクロを生成

Usage:
    regs-bitmap.py [-h] <file>

Options:
    -h       : help
"""
from docopt import docopt
import os
import re
import sys
import string

DEF_NAME_WIDTH = 20
DEF_VALUE_WIDTH = 12
DEFINE = '#define '

def spc_name(name):
    global DEF_NAME_WIDTH

    return ' ' * (DEF_NAME_WIDTH - len(name))

def out_def_bit_field(prefix, name, msb, lsb):
    global DEFINE

    nm = prefix + '_' + name
    name = nm + '_MASK'
    value = 'BITS({}, {})'.format(msb, lsb)
    print(DEFINE + name + spc_name(name) + value)

    name = nm + '_MSB'
    print(DEFINE + name + spc_name(name) + str(msb))

    name = nm + '_LSB'
    print(DEFINE + name + spc_name(name) + str(lsb))

def out_def_bit(prefix, name, bit_no):
    global DEFINE
    name = prefix + '_' + name 
    value = 'BIT({})'.format(bit_no)
    print(DEFINE + name + spc_name(name) + value)

def pick_up(ln):
    ln = ln.split(',')
    m1 = re.search(r'\[(\d+)', ln[1])
    m2 = re.search(r':(\d+)', ln[1])
    if bool(m2):
        ret = [ln[0], m1.group(1), m2.group(1)]
    else:
        ret = [ln[0], m1.group(1)]

    return ret

def out_desc(file, lines):
    global DEF_NAME_WIDTH

    pos = file.find('.')
    if pos >= 0:
        prefix = file[0:pos]
    else:
        prefix = file

    for ln in lines:
        fields = pick_up(ln)
        if len(fields) > 2:
            out_def_bit_field(prefix, fields[0], fields[1], fields[2])
        else:
            out_def_bit(prefix, fields[0], fields[1])

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
