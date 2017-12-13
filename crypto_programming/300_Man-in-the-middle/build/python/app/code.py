#!/usr/local/bin/python

import io
import os
import sys
import hashlib
import binascii
from select import select

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.asymmetric import ec
from cryptography.hazmat.primitives import serialization

from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.ciphers.algorithms import AES

def timer_readline(timeout=1, length=1024):
    rlist, _, _ = select([sys.stdin], [], [], timeout)
    if rlist:
        s = sys.stdin.buffer.read(length)
        return s
    else:
        return None

class Bus:
    def __init__(self):
        self.private_keys = [ec.generate_private_key(ec.SECP384R1(), default_backend()) for x in range(2)]
        self.public_keys = [x.public_key() for x in self.private_keys]
        self.sent_public_keys = list(self.public_keys)

        self.serialized_public_keys = [x.public_bytes(serialization.Encoding.DER, serialization.PublicFormat.SubjectPublicKeyInfo) for x in self.public_keys]

        self.iv = b"0"*16 #os.urandom(16)

    def encrypt(self, message, key):    
        backend = default_backend()
        cipher = Cipher(algorithms.AES(key), modes.CBC(self.iv), backend=backend)
        encryptor = cipher.encryptor()
        ct = encryptor.update(message) + encryptor.finalize()
        return ct

    def decrypt(self, message, key):    
        backend = default_backend()
        cipher = Cipher(algorithms.AES(key), modes.CBC(self.iv), backend=backend)
        decryptor = cipher.decryptor()
        pt = decryptor.update(message) + decryptor.finalize()
        return pt

    def _exchange(self, sender):
        receiver = sender^1
        print("[dev"+str(sender)+" to dev"+str(receiver)+"]:", flush=True, end="")
        sys.stdout.buffer.write(self.serialized_public_keys[sender])
        os.sys.stdout.buffer.flush()
        data = timer_readline(length=120) #replace regexp to remove [dev0 to dev1]??

        if data != None:
            try:
                self.sent_public_keys[sender] = serialization.load_der_public_key(data, backend=default_backend())
            except:
                print("\n[dev"+str(receiver)+" to dev"+str(sender)+"]: NG", flush=True)
                exit()
        print("\n[dev"+str(receiver)+" to dev"+str(sender)+"]: OK", flush=True)

    def exchange(self):
        self._exchange(0)
        self._exchange(1)
        print("[KBKDF: SHA256, Encryption: AES]", flush=True)
        self.exchanged_keys = [self.private_keys[x].exchange(ec.ECDH(), self.sent_public_keys[x^1]) for x in range(2)]

    def send(self, sender, message):
        receiver = sender^1
        print("[dev"+str(sender)+" to dev"+str(receiver)+"]:", flush=True, end="")
        key = hashlib.sha256(self.exchanged_keys[sender]).digest()
        ct = self.encrypt(message, key)

        sys.stdout.buffer.write(ct)
        os.sys.stdout.buffer.flush()
        return ct

    def challenge(self):
        sender = 0
        receiver = 1
        test_message = "test message"+"0"*244
        ct = self.send(sender, (test_message).encode("utf-8"))
        data = timer_readline(length=256)
        if data != None:
            ct = data
        key = hashlib.sha256(self.exchanged_keys[receiver]).digest()
        pt = self.decrypt(ct, key)
        if pt.decode("utf-8") == test_message:
            print("\n[dev"+str(receiver)+" to dev"+str(sender)+"]: OK", flush=True)
            return True
        print("\n[dev"+str(receiver)+" to dev"+str(sender)+"]: NG", flush=True)
        return False

    def send_flag(self):
        sender = 1
        receiver = 0
        flag = os.environ['flag'].encode('utf-8')
        self.send(sender, flag)

def main():
    bus = Bus()
    bus.exchange()
    if not bus.challenge():
        return
    bus.send_flag()

if __name__ == "__main__":
    main()

