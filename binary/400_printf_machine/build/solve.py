#!/usr/bin/env python
from IPython import embed
from z3 import *
import json

eq = json.load(open("eq.json"))
M = eq["M"]
B = eq["B"]
S = eq["S"]

flag = [BitVec("f%d" % i, 8) for i in range(16)]
b = [BitVec("b%d" % i, 8) for i in range(16)]
s = Solver()

for y in range(16):
    b = 0
    for x in range(16):
        b += M[x][y] * flag[S[x]]
    s.add(b == B[y])

while s.check() == sat:
    m=s.model()
    ans = [chr(m.eval(f).as_long()) for f in flag]
    print "".join(ans)
    s.add(Or([f != m.eval(f).as_long() for f in flag]))
