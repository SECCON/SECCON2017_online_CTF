#!/usr/bin/env python3
# -*- coding:utf-8 -*-

"""
make shuffled QR code patterns for Qubic Rube of SECCON 2017 Online CTF
"""

__author__ = "@shiracamus"
__version__ = "1.0"
__date__ = "09 December 2017"

import random
from vars import *

#             +-------------------------+
#             |  +-------------------+  |
#             v  v  +-------------+  |  |
#           +---------+           |  |  |
#   +------>|U1^U2^U3^|-------+   |  |  |
#   |  +--->|U4^U5^U6^|----+  |   |  |  |
#   |  |  +>|U7^U8^U9^|-+  v  v   |  |  |
# +---------+---------+---------+---------+
# |L1^L2^L3^|F1^F2^F3^|R1^R2^R3^|B1^B2^B3^|
# |L4^L5^L6^|F4^F5^F6^|R4^R5^R6^|B4^B5^B6^|
# |L7^L8^L9^|F7^F8^F9^|R7^R8^R9^|B7^B8^B9^|
# +---------+---------+---------+---------+
#   ^  ^  +-|D1^D2^D3^|<+  | |    ^  ^  ^
#   |  +----|D4^D5^D6^|<---+ |    |  |  |
#   +-------|D7^D8^D9^|<-----+    |  |  |
#           +---------+           |  |  |
#             |  |  +-------------+  |  |
#             |  +-------------------+  |
#             +-------------------------+

# rotation data: (move_from, move_to, rotate)
# the rotate means 0: 0 degree, 1: 90 degree, 2: 180 degree, 3: 270 degree

ROTATE_FRONT = (
    ('F1', 'F3', 1), ('F2', 'F6', 1), ('F3', 'F9', 1),
    ('F4', 'F2', 1), ('F5', 'F5', 1), ('F6', 'F8', 1),
    ('F7', 'F1', 1), ('F8', 'F4', 1), ('F9', 'F7', 1),
    ('U7', 'R1', 1), ('U8', 'R4', 1), ('U9', 'R7', 1),
    ('R1', 'D3', 1), ('R4', 'D2', 1), ('R7', 'D1', 1),
    ('D3', 'L9', 1), ('D2', 'L6', 1), ('D1', 'L3', 1),
    ('L9', 'U7', 1), ('L6', 'U8', 1), ('L3', 'U9', 1),
)

ROTATE_BACK = (
    ('B1', 'B3', 1), ('B2', 'B6', 1), ('B3', 'B9', 1),
    ('B4', 'B2', 1), ('B5', 'B5', 1), ('B6', 'B8', 1),
    ('B7', 'B1', 1), ('B8', 'B4', 1), ('B9', 'B7', 1),
    ('U3', 'L1', 3), ('U2', 'L4', 3), ('U1', 'L7', 3),
    ('L1', 'D7', 3), ('L4', 'D8', 3), ('L7', 'D9', 3),
    ('D7', 'R9', 3), ('D8', 'R6', 3), ('D9', 'R3', 3),
    ('R9', 'U3', 3), ('R6', 'U2', 3), ('R3', 'U1', 3),
)

ROTATE_UP = (
    ('U1', 'U3', 1), ('U2', 'U6', 1), ('U3', 'U9', 1),
    ('U4', 'U2', 1), ('U5', 'U5', 1), ('U6', 'U8', 1),
    ('U7', 'U1', 1), ('U8', 'U4', 1), ('U9', 'U7', 1),
    ('B3', 'R3', 0), ('B2', 'R2', 0), ('B1', 'R1', 0),
    ('R3', 'F3', 0), ('R2', 'F2', 0), ('R1', 'F1', 0),
    ('F3', 'L3', 0), ('F2', 'L2', 0), ('F1', 'L1', 0),
    ('L3', 'B3', 0), ('L2', 'B2', 0), ('L1', 'B1', 0),
)

ROTATE_RIGHT = (
    ('R1', 'R3', 1), ('R2', 'R6', 1), ('R3', 'R9', 1),
    ('R4', 'R2', 1), ('R5', 'R5', 1), ('R6', 'R8', 1),
    ('R7', 'R1', 1), ('R8', 'R4', 1), ('R9', 'R7', 1),
    ('U9', 'B1', 2), ('U6', 'B4', 2), ('U3', 'B7', 2),
    ('B1', 'D9', 2), ('B4', 'D6', 2), ('B7', 'D3', 2),
    ('D9', 'F9', 0), ('D6', 'F6', 0), ('D3', 'F3', 0),
    ('F9', 'U9', 0), ('F6', 'U6', 0), ('F3', 'U3', 0),
)

ROTATE_DOWN = (
    ('D1', 'D3', 1), ('D2', 'D6', 1), ('D3', 'D9', 1),
    ('D4', 'D2', 1), ('D5', 'D5', 1), ('D6', 'D8', 1),
    ('D7', 'D1', 1), ('D8', 'D4', 1), ('D9', 'D7', 1),
    ('F7', 'R7', 0), ('F8', 'R8', 0), ('F9', 'R9', 0),
    ('R7', 'B7', 0), ('R8', 'B8', 0), ('R9', 'B9', 0),
    ('B7', 'L7', 0), ('B8', 'L8', 0), ('B9', 'L9', 0),
    ('L7', 'F7', 0), ('L8', 'F8', 0), ('L9', 'F9', 0),
)

ROTATE_LEFT = (
    ('L1', 'L3', 1), ('L2', 'L6', 1), ('L3', 'L9', 1),
    ('L4', 'L2', 1), ('L5', 'L5', 1), ('L6', 'L8', 1),
    ('L7', 'L1', 1), ('L8', 'L4', 1), ('L9', 'L7', 1),
    ('U1', 'F1', 0), ('U4', 'F4', 0), ('U7', 'F7', 0),
    ('F1', 'D1', 0), ('F4', 'D4', 0), ('F7', 'D7', 0),
    ('D1', 'B9', 2), ('D4', 'B6', 2), ('D7', 'B3', 2),
    ('B9', 'U1', 2), ('B6', 'U4', 2), ('B3', 'U7', 2),
)

ROTATE_FACES = (
    ROTATE_FRONT,
    ROTATE_BACK,
    ROTATE_UP,
    ROTATE_DOWN,
    ROTATE_LEFT,
    ROTATE_RIGHT,
)

FACES = 'FBUDLR'       # Front, Back, Up, Down, Left, Right
NUMBERS = '123456789'  # 3x3
DIRECTIONS = '^>v<'    # up(0 degree), right(+90), down(+180), left(+270)


class RubicCube:

    def __init__(self):
        self.faces = {face + number: (face + number, 0)
                      for face in FACES
                      for number in NUMBERS}

    def __str__(self):
        return str({face + number: self.faces[face + number]
                    for face in FACES
                    for number in NUMBERS})

    def rotate(self, moves):
        faces = self.faces
        new_faces = faces.copy()
        for move_from, move_to, rotate in moves:
            face, direction = faces[move_from]
            new_faces[move_to] = face, (direction + rotate) % 4
        self.faces = new_faces

    def shuffle(self, times=1):
        for t in range(times):
            face = random.choice(ROTATE_FACES)
            for angle in range(random.choice((1, 2, 3))):  # times makes angle
                self.rotate(face)


def main():
    for i in range(NUM):
        cube = RubicCube()
        cube.shuffle(i)
        open('rubic/%d.txt' % (i + 1), 'w').write(str(cube))

if __name__ == '__main__':
    main()
