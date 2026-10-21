#!/usr/bin/env python3
# 随机对拍：P1627 中位数为 b 的奇数长度子区间计数
import random, subprocess, sys

MAIN = "/tmp/P1627"
BRUTE = "/tmp/P1627_brute"

bad = 0
N = 200
for it in range(N):
    n = random.randint(1, 40)
    p = list(range(1, n + 1))
    random.shuffle(p)
    b = random.choice(p)
    data = f"{n} {b}\n" + " ".join(map(str, p)) + "\n"
    a = subprocess.run([MAIN], input=data, capture_output=True, text=True).stdout.split()
    c = subprocess.run([BRUTE], input=data, capture_output=True, text=True).stdout.split()
    if a != c:
        bad += 1
        print("MISMATCH", it, data, a, c)
        if bad > 3:
            sys.exit(1)
print("done", N, "cases, mismatches =", bad)
