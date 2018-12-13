#
# (C) 2018 Hidekazu Kato
#
"""Overview:
    Generate template for various types of source files.

Usage:
    cresrc.py [-h] <file>

Options:
    -h     : help
    <file> : source file name. (*.c, *.h or *.S)
"""
from docopt import docopt
import os
import sys
import string
import datetime

OWNER = "Hidekazu Kato"
ASM_GUARD = "ASSEMBLER"

COPYRIGHT = """\
/*
 * {file}
 *
 * (C) {year} {owner}
 */
"""

H_FILE = COPYRIGHT + """\

#ifndef {inc_guard}
#define {inc_guard}

/* includes */

/* defines */

#ifndef {asm_guard}

#ifdef __cplusplus
extern \"C\" {{
#endif

/* includes */

/* defines */

/* variables */

/* functions */

#ifdef __cplusplus
}}
#endif

#endif /* {asm_guard} */

#endif /* {inc_guard} */
"""

C_FILE = COPYRIGHT + """\

#include <stdint.h>

/* defines */

/* variables */

/* functions */
"""

S_FILE = COPYRIGHT + """\
        .global     XXXX
        .section    ".text.XXXX", "ax", %progbits
        .type       XXXX, %function
        .thumb_func

        .end
"""

def inc_guard(file_name):
    return file_name.replace(os.sep, "_").replace(".", "_").upper()

def parameters(file_name):
    return {
        "owner"     : OWNER,
        "asm_guard" : ASM_GUARD,
        "year"      : datetime.date.today().year,
        "file"      : file_name,
        "inc_guard" : inc_guard(file_name),
    }

def template(file_name):
    ext = os.path.splitext(file_name)[-1]
    if ext == ".c":
        ret = C_FILE
    elif ext == ".h":
        ret = H_FILE
    elif ext == ".S":
        ret = S_FILE
    else:
        raise Exception("Illegal file extention.")
    return ret

def output(file_name):
    return template(file_name).format_map(parameters(file_name))

def main():
    args = docopt(__doc__)
    file_name = args["<file>"]
    print(output(file_name))
    return 0

if __name__ == "__main__":
    main()
