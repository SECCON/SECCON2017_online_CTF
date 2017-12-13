# -*- coding:utf-8 -*-
from Crypto.Cipher import AES


def pad(s):
    if len(s) % AES.block_size != 0:
        return s + b'\x00' * (AES.block_size - (len(s) % AES.block_size))
    return s


def get_mac(key, s):
    s = pad(s)
    cipher = AES.new(key, AES.MODE_CBC, b'\x00' * 16)
    cipher_text = cipher.encrypt(s)
    return cipher_text[-AES.block_size:]
