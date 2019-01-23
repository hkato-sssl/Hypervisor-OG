#!/bin/sh

./translate.py sysreg-a53.txt >sysreg-a53.csv
./validation.py sysreg-a53.csv
./create.py sysreg-a53.csv >sysreg-a53.h

