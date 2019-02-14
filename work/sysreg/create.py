#!/usr/bin/python3
#
# (C) 2019 Hidekazu Kato
#
"""Overview:
    CSV形式のファイルからAPIのソースコードを生成する

Usage:
    create.py [-h] <file>

Options:
    -h     : help
    <file> : CSV file name.
"""
from docopt import docopt
import os
import re
import sys
import string

TEMPLATE_W64 = """\
static inline void aarch64_write_%s(uint64_t d)
{
    __asm volatile ("msr %s, %%0" :: "r"(d) : "memory");
}
"""

TEMPLATE_W32 = """\
static inline void aarch64_write_%s(uint32_t d)
{
    __asm volatile ("msr %s, %%0" :: "r"(d) : "memory");
}
"""

TEMPLATE_R64 = """\
static inline uint64_t aarch64_read_%s(void)
{
    uint64_t d;
    __asm volatile ("mrs %%0, %s" : "=r"(d) :: "memory");
    return d;
}
"""

TEMPLATE_R32 = """\
static inline uint32_t aarch64_read_%s(void)
{
    uint32_t d;
    __asm volatile ("mrs %%0, %s" : "=r"(d) :: "memory");
    return d;
}
"""

def readlines(file_name):
    try:
        f = open(file_name, "r")
        lines = f.readlines()
    except IOError as e:
        sys.exit(e)
    finally:
        f.close()

    return lines

def output_comment(line):
    print('/*\n * ' + re.sub(r'\s+$', '', re.sub(r'#\s+', '', line)) + '\n */\n');

def output_write_function(fields):
    reg = fields[0]
    api = reg.lower()
    if fields[2] == '64':
        print(TEMPLATE_W64 % (api, reg))
    elif fields[2] == '32':
        print(TEMPLATE_W32 % (api, reg))
    else:
        print(fields)
        raise Exception("Illegal register length.")

def output_read_function(fields):
    reg = fields[0]
    api = reg.lower()
    if fields[2] == '64':
        print(TEMPLATE_R64 % (api, reg))
    elif fields[2] == '32':
        print(TEMPLATE_R32 % (api, reg))
    else:
        print(fields)
        raise Exception("Illegal register length.")

def output_function(line):
    fields = line.split(',');
    if len(fields) > 3:
        print('/*', fields[3], '*/\n')
    if re.search(r'RW|WO', fields[1]):
        output_write_function(fields)
    if re.search(r'RW|RO', fields[1]):
        output_read_function(fields)

def create(lines):
    for ln in lines:
        ln = ln[0:-1]               # remove a last character
        if len(ln) > 0:
            if re.match(r'#', ln):    # comment
                output_comment(ln)
            else:
                output_function(ln)

def main():
    args = docopt(__doc__)
    file_name = args["<file>"]
    lines = readlines(file_name)
    create(lines)
    return 0

if __name__ == "__main__":
    main()
