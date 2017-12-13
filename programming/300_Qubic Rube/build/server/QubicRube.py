#!/usr/bin/env python
# -*- coding:utf-8 -*-

"""
Qubic Rube web server of SECCON 2017 Online CTF
"""

__author__ = "@shiracamus"
__version__ = "1.0"
__date__ = "09 December 2017"

# sudo apt install libev-dev
# sudo pip install bottle
# sudo pip install bjoern

from bottle import default_app, route, template, static_file, request
from datetime import datetime
from sys import stdout

HOST = '0.0.0.0'
PORT = 33654    # 3x3x6=54
TOP = '01000000000000000000'

def log(message):
    stdout.write( datetime.now().strftime('%Y/%m/%d %H:%M:%S')
                + ' ' + str(request.remote_addr)
                + ' ' + message
                + '\n'
                )
    stdout.flush()

@route('/')
def index():
    return template('index', path=TOP)

@route('/<path:re:[0-9a-f]{20}>')
def next(path):
    return template('index', path=path)

@route('/js/three.min.js')
def threejs():
    return static_file('three.min.js', root="./js")

@route('/images/<file_path:path>')
def image(file_path):
    if file_path.endswith('_B.png'):
        log(file_path)
    return static_file(file_path, root='./images')

import bjoern
bjoern.run(default_app(), host=HOST, port=PORT)
