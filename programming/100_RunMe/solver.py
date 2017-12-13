import sys
from functools import lru_cache
sys.setrecursionlimit(99999)
@lru_cache(maxsize=None)
def f(n):
    return n if n < 2 else f(n-2) + f(n-1)
print("SECCON{" + str(f(11011))[:32] + "}")
