#!/usr/bin/env python
# -*- coding:utf-8 -*-

"""
make web page pathes for Qubic Rube of SECCON 2017 Online CTF
"""

__author__ = "@shiracamus"
__version__ = "1.0"
__date__ = "09 December 2017"

import os
from vars import *

# The first byte of the path is a sequence number.
# The follow bytes are random numbers.
# But, the follows are all 0 for sequence number 1.
# So, this script makes pathes after sequence number 1.

FOLLOW_BYTES = (PATH_LENGTH - 2) // 2  # -2 for the sequence number
for number in range(2, NUM + 1):
    print(('%02d' % number) + os.urandom(FOLLOW_BYTES).encode('hex'))
