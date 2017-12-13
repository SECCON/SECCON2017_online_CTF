import os
import string
import random
import binascii
from simon import SimonCipher

key_plain = os.environ['flag'].encode("utf-8")
key_encoded = int.from_bytes(key_plain, "big")

my_simon = SimonCipher(key_encoded)

my_plaintext = ''.join(random.SystemRandom().choice(string.ascii_lowercase + string.ascii_uppercase + string.digits) for _ in range((8))).encode("utf-8")
my_plaintext_encoded = int.from_bytes(my_plaintext, "big")

simon_ciphertext = my_simon.encrypt(my_plaintext_encoded)
simon_plaintext = my_simon.decrypt(simon_ciphertext)

print(key_plain)
print(my_plaintext_encoded)
print(simon_plaintext)

print(my_plaintext)
print(simon_plaintext.to_bytes(8, "big").decode("utf-8"))

with open("/usr/src/app/data/plain", "wb") as fd:
    fd.write(my_plaintext)

with open("/usr/src/app/data/cipher", "wb") as fd:
    fd.write(simon_ciphertext.to_bytes(16, "big"))




