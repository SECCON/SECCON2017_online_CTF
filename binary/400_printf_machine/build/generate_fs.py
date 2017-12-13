#!/usr/bin/env python
from string import Template
import sys

storage_size = 32
addr_base = 1
val_base = addr_base + storage_size

#the value of m[1] is zero

def sgl_st(dst):
    t = Template("%${pos}$$hhn")
    return t.substitute(pos=addr_base+dst)

def sgl_add(src):
    t = Template("%2$$*${pos}$$s") # depends on m[1]==0
    return t.substitute(pos=val_base+src)

def sgl_addi(imm):
    return "%%2$%ds" % imm

def sgl_strlen(src):
    t = Template("%${pos}$$s")
    return t.substitute(pos=addr_base+src)

def mlt_cmp(dst, src, imm):
    retval = [
        "".join([
            sgl_add(src),
            sgl_addi((256 - imm)%256),
            sgl_st(0),
        ]),
        "".join([
            sgl_strlen(0),  # reg = strlen(&m[0])
            sgl_st(dst),    # m[dst] = reg
        ]),
    ]
    return retval

def mlt_mul(dst, src, imm, wm=8):
    retval = []
    cursor = 1
    retval.append("".join([
        sgl_st(dst),        # mem[dst] = 0
        sgl_add(src),       # reg = mem[src]
        sgl_st(wm),         # mem[wm] = reg
    ]))
    while imm >= cursor:
        if imm & cursor:
            retval.append("".join([
                sgl_add(dst),
                sgl_add(wm),
                sgl_st(dst),
            ]))
        cursor *= 2
        retval.append("".join([
            sgl_add(wm),    # reg = mem[wm]
            sgl_add(wm),    # reg += mem[wm]
            sgl_st(wm),     # mem[wm] = reg
        ]))
    return retval

def mlt_nonzero(dst, src):
    retval = [
        "".join([
            sgl_st(0),      # m[0] = 0
            sgl_add(src),   # reg = m[src]
            sgl_st(0),      # m[0] = reg
        ]),
        "".join([
            sgl_strlen(0),  # reg = strlen(&m[0])
            sgl_st(dst),    # m[dst] = reg
        ]),
    ]
    return retval

def mlt_inverse(dst, src):
    retval = [
        "".join([
            sgl_st(0),      # m[0] = 0
            sgl_add(src),   # reg = m[src]
            sgl_addi(255),  # reg += 255
            sgl_st(0),      # m[0] = reg
        ]),
        "".join([
            sgl_strlen(0),  # reg = strlen(&m[0])
            sgl_st(dst),    # m[dst] = reg
        ]),
    ]
    return retval

import json
eq = json.load(open("eq.json"))
M = eq["M"]
B = eq["B"]
S = eq["S"]

prog = []
var_cnt = 15
var_sum = 2
var_tmp = 3
var_rslt = 4

for x in range(16):
    if x == S[x]:
        continue
    prog += [sgl_add(16+x) + sgl_st(var_tmp)]
    prog += [sgl_add(16+S[x]) + sgl_st(16+x)]
    prog += [sgl_add(var_tmp) + sgl_st(16+S[x])]
    S[S.index(x)] = S[x]
    S[x] = x

for y in range(16):
    prog += [sgl_st(var_sum)] # m[var_sum] = 0
    for x in range(16):
        #prog += mlt_mul(var_tmp, 16+S[x], M[x][y]) # m[var_tmp] = m[16+S[x]] * M[x][y]
        prog += mlt_mul(var_tmp, 16+x, M[x][y]) # m[var_tmp] = m[16+S[x]] * M[x][y]
        prog += [sgl_add(var_sum) + sgl_add(var_tmp) + sgl_st(var_sum)] # m[var_sum] += m[var_tmp]
    prog += mlt_cmp(var_rslt, var_sum, B[y]) # m[var_rslt] = (m[var_sum] == B[y]?0:1)
    prog += [sgl_add(var_cnt) + sgl_add(var_rslt) + sgl_st(var_cnt)] # m[var_cnt] += m[var_rslt]

open("default.fs", "w").write("\n".join(prog))

