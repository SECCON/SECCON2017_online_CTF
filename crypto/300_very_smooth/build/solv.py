#! python 
#solv.py : This program needs s.modulus file

import sys
import struct
from Crypto.PublicKey import RSA
from Crypto.Util.number import inverse
from Crypto.Util.number import GCD

f = open('s.modulus','r')
buf = f.readline()
f.close()

n_text = buf[8:]
n = int (n_text,16)

#Solving  p-1 method 
seed = 2 #any value
B = 10000 #B-smooth
b = 1 #count of B-smooth
gcd_value = 1
while gcd_value <= 1 :
	b = b + 1
	if (b >= B):
		break
	seed = pow(seed, b, n)
	gcd_value = GCD(seed - 1, n)
# p is GCD value except 1
p = gcd_value
q = n // p
#make private key
e = 65537
d = inverse(e, (p-1)*(q-1))
#make key set
keys = RSA.construct((n,e,d))
#write out keyset s.key in PEM
f = open('s.key','w')
f.write(str(keys.exportKey('PEM')))
f.close()
