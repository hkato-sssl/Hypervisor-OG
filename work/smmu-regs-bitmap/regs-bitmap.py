#!/usr/bin/python3
#
# (C) 2019 Hidekazu Kato
#
"""Overview:
    CSVファイルからregister bitmap定義マクロを生成

Usage:
    regs-bitmap.py [-h] <file> [-w <width>]

Options:
    -h       : help
    -w       : width of a definition name (default: 28)
"""
from docopt import docopt
import os
import re
import sys
import string

DEF_NAME_WIDTH = 28
DEF_VALUE_WIDTH = 12
DEFINE = '#define '

def spc_name(name):
    global DEF_NAME_WIDTH

    n = DEF_NAME_WIDTH - len(name)
    if n <= 0:
        raise Exception("DEF_NAME_WIDTH is too small. (default: {})".format(DEF_NAME_WIDTH));

    return ' ' * n;

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

    name = nm + '(n)'
    value = '(((n) << {}_LSB) & {}_MASK)'.format(nm, nm)
    print(DEFINE + name + spc_name(name) + value)

    name = 'EXTRACT_{}(d)'.format(nm);
    value = '(((d) & {}_MASK) >> {}_LSB)'.format(nm, nm);
    print(DEFINE + name + spc_name(name) + value)

def out_def_bit(prefix, name, bit_no):
    global DEFINE
    name = prefix + '_' + name 
    value = 'BIT({})'.format(bit_no)
    print(DEFINE + name + spc_name(name) + value)

def out_def_res1(prefix, bits):
    global DEFINE
    name = prefix + '_RES1' 
    value = '(' + bits[1:-2] + ')'
    print(DEFINE + name + spc_name(name) + value)

def parse_bit_field(f):
    m1 = re.search(r'\[(\d+)', f)
    if bool(m1):
    	m2 = re.search(r':(\d+)', f)
    	if bool(m2):
        	ret = ' BITS({}, {}) |'.format(m1.group(1), m2.group(1))
    	else:
        	ret = ' BIT({}) |'.format(m1.group(1))
    else:
        ret = ''

    return ret

def pick_up(ln):
    ln = ln.strip(' ').split(',')
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
    global DEF_NAME_WIDTH
    res1 = ''

    pos = file.find('.')
    if pos >= 0:
        prefix = file[0:pos]
    else:
        prefix = file

    for ln in lines:
        fields = pick_up(ln)
        fl = len(fields)
        if (fl == 1):
            res1 += parse_bit_field(fields[0])
        elif fl == 2:
            out_def_bit(prefix, fields[0], fields[1])
        elif fl > 2:
            out_def_bit_field(prefix, fields[0], fields[1], fields[2])

    if len(res1):
        out_def_res1(prefix, res1)

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
    if args['-w']:
        DEF_NAME_WIDTH = int(args['<width>'])
    main(args)
