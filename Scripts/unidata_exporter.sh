#!/bin/bash

cat ${1} | awk --field-separator=";" '{a=strtonum("0x"$13); b=strtonum("0x"$14); if (!(a == 0 && b == 0)) {printf("%d, %d, %d,\n", strtonum("0x"$1), a, b)}}' 


