#!/usr/bin/env python3
# 随机对拍：P1628 按字典序输出以 T 为前缀的单词
import random, subprocess, sys

MAIN = "/tmp/P1628"
BRUTE = "/tmp/P1628_brute"

bad = 0
N = 200
alpha = "abc"
for it in range(N):
    n = random.randint(1, 12)
    words = ["".join(random.choice(alpha) for _ in range(random.randint(1, 5))) for _ in range(n)]
    t = "".join(random.choice(alpha) for _ in range(random.randint(1, 3)))
    data = str(n) + "\n" + "\n".join(words) + "\n" + t + "\n"
    a = subprocess.run([MAIN], input=data, capture_output=True, text=True).stdout.split()
    b = subprocess.run([BRUTE], input=data, capture_output=True, text=True).stdout.split()
    ref = sorted(w for w in words if w.startswith(t))
    if a != b or a != ref:
        bad += 1
        print("MISMATCH", it, data, a, b, ref)
        if bad > 3:
            sys.exit(1)
print("done", N, "cases, mismatches =", bad)
