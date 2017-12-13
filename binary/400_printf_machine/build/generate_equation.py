#!/usr/bin/env python
from random import randint, shuffle
import json
import numpy as np

"       0123456789abcdef"
flag = "Tur!n9-C0mpl3t3?"

equation = {
    "M": [],
    "B": [0] * 16,
    "S": range(16),
}

shuffle(equation["S"])

for _ in range(16):
    equation["M"].append([randint(0, 255) for _ in range(16)])

for y in range(16):
    for x in range(16):
        equation["B"][y] += equation["M"][x][y] * ord(flag[equation["S"][x]])
    equation["B"][y] &= 255

open("eq.json", "w").write(json.dumps(equation))
