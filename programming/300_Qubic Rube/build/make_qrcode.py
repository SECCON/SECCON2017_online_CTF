#!/usr/bin/env python
# -*- coding:utf-8 -*-

"""
make QR code images for Qubic Rube of SECCON 2017 Online CTF
"""

__author__ = "@shiracamus"
__version__ = "1.0"
__date__ = "09 December 2017"

import qrcode       # sudo pip install qrcode
from vars import *

FLAG = open("../flag.txt").read().strip()

MESSAGES = {
    'welcome': 'SECCON 2017 Online CTF',
    'title': 'Qubic Rube',
    'url': 'Next URL is:',
    'havefun': 'Have fun!',
    'gogo': 'Go! Go!',
    'grats': 'Congratulations!',
    'flag': 'The flag is:',
}

LEN = max((
    URL_LENGTH,
    len(FLAG),
    max(map(len, MESSAGES.values())),
))


def make_qrcode(text):
    qr = qrcode.QRCode(box_size=6)
    qr.add_data(text.ljust(LEN), optimize=0)
    return qr.make_image()


def make_messages():
    for title, message in MESSAGES.items():
        make_qrcode(message).save('qrcode/%s.png' % title)


def make_numbers():
    for i in range(1, NUM + 1):
        make_qrcode('No. %d / %d' % (i, NUM)).save('qrcode/no%d.png' % i)


def make_pathes():
    with open('pathes.txt') as pathes:
        for path in pathes:
            path = path.strip()
            make_qrcode(URL_FORMAT % path).save('qrcode/%s.png' % path)


def make_flag():
    make_qrcode(FLAG).save('qrcode/seccon.png')

if __name__ == '__main__':
    make_messages()
    make_numbers()
    make_pathes()
    make_flag()
