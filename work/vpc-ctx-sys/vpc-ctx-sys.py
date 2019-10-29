#!/usr/bin/python3
#
# (C) 2019 Hidekazu Kato
#
"""Overview:
    CSVファイルからSystem Registerに関するContext Switch
    処理APIを自動生成する。
    変換元のCSVファイルはスクリプト内部に記述されて いる。
    A53フィールドが〇のレジスタを生成対象とする

Usage:
    vcpu-ctx-sys.py [-h]

Options:
    -h       : help
"""
from docopt import docopt
import os
import re
import sys
import string

HEADER_SPC = ' ' * 8
PREFIX = 'VPC_'
FILES = [ 'EL0-EL1', 'EL2' ]

LOAD_FUNC = 'vpc_ctx_load_system_registers'
STORE_FUNC = 'vpc_ctx_store_system_registers'

FUNC_PROLOGUE = """\
        .global         $func
        .section        ".text.$func", "ax", %progbits
        .type           $func, %function
        .balign         4
        // x0 - the address of Register Array
$func:
        add             x1, x0, #($reg * 8)

"""

FUNC_EPILOGUE = """\

        ret

"""

OPS_LDP = """\
        ldp             x2, x3, [x1], #16
        msr             $reg1, x2
        msr             $reg2, x3
"""

OPS_LDR = """\
        ldr             x2, [x1]
        msr             $reg, x2
"""

OPS_STP = """\
        mrs             x2, $reg1
        mrs             x3, $reg2
        stp             x2, x3, [x1], #16
"""

OPS_STR = """\
        mrs             x2, $reg
        str             x2, [x1]
"""

T_FUNC_PROLOGUE = string.Template(FUNC_PROLOGUE)
T_OPS_LDP = string.Template(OPS_LDP)
T_OPS_LDR = string.Template(OPS_LDR)
T_OPS_STP = string.Template(OPS_STP)
T_OPS_STR = string.Template(OPS_STR)

def readlines(file_name):
    try:
        f = open(file_name, "r")
        lines = f.readlines()
    except IOError as e:
        sys.exit(e)
    finally:
        f.close()

    return lines

def out_func_prologue(func, reg):
    global T_FUNC_PROLOGUE
    return T_FUNC_PROLOGUE.substitute(func=func, reg=reg)

def out_func_epilogue():
    global FUNC_EPILOGUE
    return FUNC_EPILOGUE

def out_ops_dual(template, reg1, reg2):
    return template.substitute(reg1 = reg1, reg2 = reg2)

def out_ops_single(template, reg):
    return template.substitute(reg = reg)

def validate_registers(lines):
    regs = []
    for ln in lines:
        fields = ln.split(',')
        if fields[4] == '〇':
            regs.append(fields[0])

    return regs

def out_function(func, t1, t2, regs):
    outs = []
    outs.append(out_func_prologue(func, regs[0]))
    n = (len(regs) | 1) ^ 1
    for i in range(0, n, 2):
        outs.append(out_ops_dual(t2, regs[i], regs[i + 1]))
    if (len(regs) & 1) != 0:
        outs.append(out_ops_single(t1, regs[-1]))
    outs.append(out_func_epilogue())

    return outs

def out_load_function(regs):
    global T_OPS_LDR
    global T_OPS_LDP
    return out_function(LOAD_FUNC, T_OPS_LDR, T_OPS_LDP, regs)

def out_store_function(regs):
    global T_OPS_STR
    global T_OPS_STP
    return out_function(STORE_FUNC, T_OPS_STR, T_OPS_STP, regs)

def create_desc(lines):
    regs = validate_registers(lines)
    outs = out_load_function(regs)
    outs += out_store_function(regs)
    for ln in outs:
        print(ln, end = '')

def main():
    global files

    lines = []
    for fn in FILES:
        lines += readlines(fn + '.csv')
    create_desc(lines)

    return 0

if __name__ == "__main__":
    main()
