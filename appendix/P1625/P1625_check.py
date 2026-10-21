#!/usr/bin/env python3
# 随机对拍：P1625 求和（分数求和）
# 判据：标程输出的 (X,Y) 必须与暴力逐项累加得到的分数完全相等，且 gcd(X,Y)=1
import random, subprocess, sys, math

MAIN = "/tmp/P1625"
BRUTE = "/tmp/P1625_brute"

bad = 0
N = 150
for it in range(N):
    n = random.randint(1, 8)
    m = random.randint(2, max(2, 11 - n))
    data = f"{n} {m}\n"
    a = subprocess.run([MAIN], input=data, capture_output=True, text=True).stdout.split()
    b = subprocess.run([BRUTE], input=data, capture_output=True, text=True).stdout.split()
    if a != b:
        bad += 1
        print("MISMATCH", data.strip(), a, b)
        if bad > 3:
            sys.exit(1)
        continue
    X, Y = int(a[0]), int(a[1])
    if math.gcd(X, Y) != 1:
        bad += 1
        print("NOT REDUCED", data.strip(), X, Y)
        if bad > 3:
            sys.exit(1)
print("done", N, "cases, mismatches =", bad)
