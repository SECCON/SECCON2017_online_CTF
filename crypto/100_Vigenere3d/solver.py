import sys
s = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz_{}"
p = "SECCON{"
c = "POR4dnyTLHBfwbxAAZhe}}ocZR3Cxcftw9"
k = []
for i in range(len(p)):
	P = s.find(p[i])
	C = s.find(c[i])
	if P <= C:
		k.append(C - P)
		continue
	k.append((len(s) - P) + C)
k += k[::-1]
p = ""
for i in range(len(c)):
	num = s.find(c[i]) - k[i % len(k)]
	if num < 0:
		num = len(s) + num
	p += s[num % len(s)]
print p
