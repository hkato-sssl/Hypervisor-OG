#!/usr/bin/python3
#
# (C) 2019 Hidekazu Kato
#
"""Overview:
    CSV形式のファイルからAPIのソースコードを生成する

Usage:
    creelx.py [-h] <file>

Options:
    -h     : help
    <file> : CSV file name.
"""
from docopt import docopt
import os
import re
import sys
import string

SOURCE_MACRO = """\
        .macro          BR_EL, table
        adr             ip0, \\table
        mrs             ip1, CurrentEL
        add             ip0, ip0, ip1, lsl #1
        ldr             ip1, [ip0]
        br              ip1
        .endm

        .macro          ILLEGAL_OP
        svc             #0xffff
        .endm
"""

SOURCE_END = """\
        .end
"""

TEMPLATE_GLOBAL_WRITE_DESC = """\
        .global         aarch64_write_{lreg}"""

TEMPLATE_GLOBAL_READ_DESC = """\
        .global         aarch64_read_{lreg}"""

TEMPLATE_WRITE_FUNCTION = """\
        .section        ".text.aarch64_write_{lreg}"
        .type           aarch64_write_{lreg}, %function
        .balign         4
aarch64_write_{lreg}:
        BR_EL           000f
        .balign         8
000:    .quad           999f
        .quad           write_{lreg}_el1
        .quad           write_{lreg}_el2
        .quad           write_{lreg}_el3
999:
        ILLEGAL_OP
write_{lreg}_el1:
        msr             {ureg}_EL1, x0
        ret
write_{lreg}_el2:
        msr             {ureg}_EL2, x0
        ret
write_{lreg}_el3:
        msr             {ureg}_EL3, x0
        ret
"""

TEMPLATE_READ_FUNCTION = """\
        .section        ".text.aarch64_read_{lreg}"
        .type           aarch64_read_{lreg}, %function
        .balign         4
aarch64_read_{lreg}:
        BR_EL           000f
        .balign         8
000:    .quad           999f
        .quad           read_{lreg}_el1
        .quad           read_{lreg}_el2
        .quad           read_{lreg}_el3
999:
        ILLEGAL_OP
read_{lreg}_el1:
        mrs             x0, {ureg}_EL1
        ret
read_{lreg}_el2:
        mrs             x0, {ureg}_EL2
        ret
read_{lreg}_el3:
        mrs             x0, {ureg}_EL3
        ret
"""

globals = []
functions = []

def readlines(file_name):
    try:
        f = open(file_name, "r")
        lines = f.readlines()
    except IOError as e:
        sys.exit(e)
    finally:
        f.close()

    return lines

def output_write_function(reg):
    globals.append(TEMPLATE_GLOBAL_WRITE_DESC.format(lreg=reg))
    functions.append(TEMPLATE_WRITE_FUNCTION.format(lreg=reg, ureg=reg.upper()))

def output_read_function(reg):
    globals.append(TEMPLATE_GLOBAL_READ_DESC.format(lreg=reg))
    functions.append(TEMPLATE_READ_FUNCTION.format(lreg=reg, ureg=reg.upper()))

def output_function(line):
    fields = line.split(',');
    if len(fields) == 2:
        reg = fields[0].lower()
        access = fields[1].upper();
        if access == "RW" or access == "WO":
            output_write_function(reg)
        if access == "RW" or access == "RO":
            output_read_function(reg)
    else:
        raise Exception("Illegal format.")

def creelx(lines):
    for ln in lines:
        ln = ln[0:-1]               # remove a last character
        if len(ln) > 0:
            output_function(ln)

def output_array(a):
    for ln in a:
        print(ln)

def main():
    args = docopt(__doc__)
    file_name = args["<file>"]
    lines = readlines(file_name)
    creelx(lines)
    output_array(globals)
    print();
    print(SOURCE_MACRO)
    output_array(functions)
    print(SOURCE_END)
    return 0

if __name__ == "__main__":
    main()
