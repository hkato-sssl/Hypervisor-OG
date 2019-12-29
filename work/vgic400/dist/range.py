#!/usr/bin/python3
#
# (C) 2019 Hidekazu Kato
#
"""Overview:
    CSVファイル内で複数行で記述されているレジスタ定義をまとめる

Usage:
    range.py [-h] <file>

Options:
    -h       : help
"""
from docopt import docopt
import os
import string

def out_source(array):
    n = 0;
    for ln in array:
        v1 = ln[0]
        v2 = ln[1]
        name = ln[2]
        if (v1 == v2):
            str = 'if (reg == 0x{:04x}) {{'.format(v1)
        else:
            str = 'if ((0x{:04x} <= reg) && (reg <= 0x{:04x})) {{'.format(v1, v2+3)

        if (n > 0):
            str = '} else ' + str
        n += 1
        m = len(str) % 4
        str = str + ' ' * (4 - m) + '/* ' + name + ' */'
        print(str)
    print('}')

def update_line(array, str1):
    pos = str1.find('-')
    if (pos < 0):
        v = int(str1, 16)
    else:
        v = int(str1[pos+1:], 16)
    array[-1][1] = v

    return array

def new_line(array, str1, str2):
    pos = str1.find('-')
    if (pos < 0):
        v = int(str1, 16)
        array.append([v, v, str2])
    else:
        v1 = int(str1[0:pos], 16)
        v2 = int(str1[pos+1:], 16)
        array.append([v1, v2, str2])

    return array

def format_fields(lines):
    array = []
    for ln in lines:
        pos1 = ln.find(',')
        pos2 = ln.find(',', pos1+1)
        str1 = ln[0:pos1]
        str2 = ln[pos1+1:pos2]
        if str2 != '':
            array = new_line(array, str1, str2)
        else:
            array = update_line(array, str1)

    return array

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
    array = format_fields(lines)
    out_source(array)

if __name__ == "__main__":
    args = docopt(__doc__)
    main(args)
