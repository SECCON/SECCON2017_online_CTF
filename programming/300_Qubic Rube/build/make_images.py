#!/usr/bin/env python
# -*- coding:utf-8 -*-

"""
make images for Qubic Rube web server of SECCON 2017 Online CTF
"""

__author__ = "@shiracamus"
__version__ = "1.0"
__date__ = "09 December 2017"

import random
from PIL import Image   # sudo apt install python-imaging
from vars import *


def RGB(rgb):
    return ((rgb >> 16) & 0xff, (rgb >> 8) & 0xff, rgb & 0xff)

COLORS = {
    'U': RGB(0xFFFFFF),  # White
    'L': RGB(0xFF5800),  # Orange
    'F': RGB(0x009E60),  # Green
    'R': RGB(0xC41E3A),  # Red
    'B': RGB(0xFFD500),  # Yellow
    'D': RGB(0x0051BA),  # Blue
}
BLACK = RGB(0)
FACES = 'ULFRBD'
NUMBERS = '123456789'


def shuffle_colors():
    colors = list(COLORS.values())
    random.shuffle(colors)
    for face, color in zip(FACES, colors):
        COLORS[face] = color


def paste_qr(image, qr):
    width, height = image.size
    for y in range(height):
        for x in range(width):
            if qr.getpixel((x, y)) == 0:
                image.putpixel((x, y), BLACK)


def make_rubic_faces(qrnames, rubic):
    qrs = [Image.open(qr) for qr in qrnames]
    width, height = size = qrs[0].size
    w = h = width // 3
    faces = [Image.new('RGB', (w * 3, h * 3), COLORS[face]) for face in FACES]
    crops = {}
    for FACE, face, qr in zip(FACES, faces, qrs):
        paste_qr(face, qr)
        for n, NUMBER in enumerate(NUMBERS):
            x, y = (n % 3) * w, (n // 3) * h
            crops[FACE + NUMBER] = face.crop((x, y, x + w, y + h))
    moved = {move_to: crops[FACE + NUMBER].rotate(rotate * 90)
             for FACE in FACES
             for NUMBER in NUMBERS
             for move_to, rotate in [rubic[FACE + NUMBER]]}
    for face, FACE in enumerate(FACES):
        for n, NUMBER in enumerate(NUMBERS):
            faces[face].paste(
                moved[FACE + NUMBER], ((n % 3) * w, (n // 3) * h))
    return faces


def make_image(number, faces, path):
        with open('rubic/%s.txt' % number) as rubic:
            faces = make_rubic_faces(faces, eval(rubic.read()))
        for FACE, face in zip(FACES, faces):
            face.save('server/images/%s_%s.png' % (path, FACE))


def make_images():
    with open(PATH_FILENAME) as f:
        pathes = f.readlines()
    number = 1
    path = FIRST_PATH
    for next_path in pathes:
        next_path = next_path.strip()
        images = [
            'qrcode/welcome.png',         # SECCON 2017 Online CTF
            'qrcode/title.png',           # Qubic Rube
            'qrcode/no%d.png' % number,   # No. number / NUM
            'qrcode/url.png',             # Next URL is:
            'qrcode/%s.png' % next_path,  # http://x.x.x.x/path
            'qrcode/havefun.png',         # Have fun!
        ]
        if number > 10:
            shuffle_colors()
        if number > 20:
            random.shuffle(images)
        if number > 40:
            images[images.index('qrcode/havefun.png')] = 'qrcode/gogo.png'
        make_image(number, images, path)
        number += 1
        path = next_path
    # LAST: the flag
    images = [
        'qrcode/welcome.png',        # SECCON 2017 Online CTF
        'qrcode/title.png',          # Cubic Rube
        'qrcode/no%d.png' % number,  # No. number / NUM
        'qrcode/grats.png',          # Congraturations!
        'qrcode/flag.png',           # The flag is:
        'qrcode/seccon.png',         # SECCON{...}
    ]
    make_image(number, images, path)

if __name__ == '__main__':
    make_images()
