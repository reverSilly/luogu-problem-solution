#!/usr/bin/env python3
# 随机对拍：P1630 求 1^b+...+a^b mod 10000
# 小 a：与暴力逐项求幂累加（P1630_brute）比对
# 大 a：b<=3 时用闭式公式 sum i = a(a+1)/2, sum i^2 = a(a+1)(2a+1)/6, sum i^3 = (a(a+1)/2)^2 独立验证分块周期
import random, subprocess, sys

MAIN = "/tmp/P1630"
BRUTE = "/tmp/P1630_brute"

def closed(a, b):
    if b == 1:
        return a * (a + 1) // 2 % 10000
    if b == 2:
        return a * (a + 1) * (2 * a + 1) // 6 % 10000
    return (a * (a + 1) // 2) ** 2 % 10000

bad = 0
N = 120
for it in range(N):
    small = [(random.randint(1, 20), random.randint(1, 6)) for _ in range(3)]
    data = str(len(small)) + "\n" + "\n".join(f"{a} {b}" for a, b in small) + "\n"
    x = subprocess.run([MAIN], input=data, capture_output=True, text=True).stdout.split()
    y = subprocess.run([BRUTE], input=data, capture_output=True, text=True).stdout.split()
    if x != y:
        bad += 1
        print("SMALL MISMATCH", data, x, y)
        if bad > 3:
            sys.exit(1)
    big = [(random.randint(9990, 20050), random.randint(1, 6)),
           (random.randint(1, 10 ** 9), random.randint(1, 3)),
           (10 ** 9, random.randint(1, 3))]
    data = str(len(big)) + "\n" + "\n".join(f"{a} {b}" for a, b in big) + "\n"
    out = subprocess.run([MAIN], input=data, capture_output=True, text=True).stdout.split()
    for (a, b), got in zip(big, out):
        ref = closed(a, b) if b <= 3 else sum(pow(i, b, 10000) for i in range(1, a + 1)) % 10000
        if int(got) != ref:
            bad += 1
            print("BIG MISMATCH", a, b, got, ref)
            if bad > 3:
                sys.exit(1)
print("done", N, "cases, mismatches =", bad)
