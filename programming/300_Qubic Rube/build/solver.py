#!/usr/bin/env python2
# -*- coding:utf-8 -*-

"""
solver for Qubic Rube of SECCON 2017 Online CTF
"""

__author__ = "@shiracamus"
__version__ = "1.0"
__date__ = "09 December 2017"

from urllib2 import urlopen
from PIL import Image               # sudo apt install python-imaging
from pyzbar.pyzbar import decode    # sudo apt install python-zbar
from itertools import product       # sudo pip install pyzbar

HOST = 'qubicrube.pwn.seccon.jp'
PORT = 33654

BLACK = (0, 0, 0)


def load_images(home, url):
    html = urlopen(url).read()
    image_urls = [home + text[:text.index('"')]
                  for text in html.split('loader.load( "')[1:]]
    return [Image.open(urlopen(url)) for url in image_urls]


def divide_3x3(image):
    width, height = size = image.size
    w, h = width // 3, height // 3
    parts = [image.crop((x, y, x + w, y + w))
             for y in range(0, h * 3, h)
             for x in range(0, w * 3, w)]
    return parts


def join_3x3(parts):
    w, h = parts[0].size
    image = Image.new('RGB', (w * 3, h * 3), BLACK)
    for i, part in enumerate(parts):
        image.paste(part, ((i % 3) * w, (i // 3) * h))
    return image


def color_except_black(image):
    width, height = image.size
    for y in range(height):
        for x in range(width):
            pixel = image.getpixel((x, y))
            if pixel != BLACK:
                return pixel
    return BLACK


def find_up(parts, target):
    width, height = target.size
    top = [target.getpixel((x, 0)) for x in range(width)]
    y = height - 1
    for part in parts:
        for direction in range(4):
            if all(part.getpixel((x, y)) == top[x] for x in range(width)):
                return part
            part = part.rotate(90)


def solve_rubic(faces):
    # collect center pieces and make color list dictionary
    # the top of the list is the center piece each color.
    centers = [face[4] for face in faces]
    colors = {color_except_black(center): [center] for center in centers}
    for face in faces:
        for i in 0, 1, 2, 3, 5, 6, 7, 8:
            colors[color_except_black(face[i])] += [face[i]]
    Fs = colors.values()
    Cs = [parts[0] for parts in Fs]
    Os = [parts[1:] for parts in Fs]
    # collect neighbor pieces of the center (up, down, right, left)
    Us = [find_up(o, c) for o, c in zip(Os, Cs)]
    Ds = [find_up(o, c.rotate(180)).rotate(180) for o, c in zip(Os, Cs)]
    Rs = [find_up(o, c.rotate(90)).rotate(-90) for o, c in zip(Os, Cs)]
    Ls = [find_up(o, c.rotate(-90)).rotate(90) for o, c in zip(Os, Cs)]
    # collect corner pieces (up left, up right, down left, down right)
    ULs = [find_up(o, l) for o, l in zip(Os, Ls)]
    URs = [find_up(o, r) for o, r in zip(Os, Rs)]
    DLs = [find_up(o, l.rotate(180)).rotate(180) for o, l in zip(Os, Ls)]
    DRs = [find_up(o, r.rotate(180)).rotate(180) for o, r in zip(Os, Rs)]
    # sort pieces for readable QR code
    faces = map(list, zip(ULs, Us, URs, Ls, Cs, Rs, DLs, Ds, DRs))
    return faces


def decode_qrcode(qr):
    return str(decode(qr)[0][0].decode('utf-8', 'ignore')).strip()


def read_texts(home, url):
    faces = load_images(home, url)
    faces = [divide_3x3(face) for face in faces]
    faces = solve_rubic(faces)
    faces = [join_3x3(face) for face in faces]
    for face in faces:
        yield decode_qrcode(face)


def solve(host, port):
    home = "http://%s:%s" % (host, port)
    url = home + '/'
    while url:
        print("Reading... %s" % url)
        texts = list(read_texts(home, url))
        print('\n'.join(texts))
        urls = [text for text in texts if text.startswith('http://')]
        url = urls and urls[0] or None

if __name__ == '__main__':
    solve(HOST, PORT)
