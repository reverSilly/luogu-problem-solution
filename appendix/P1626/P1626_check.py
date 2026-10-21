#!/usr/bin/env python3
# 随机对拍：P1626 象棋比赛（K 场对局最小等级差和）
import random, subprocess, sys

MAIN = "/tmp/P1626"
BRUTE = "/tmp/P1626_brute"

bad = 0
N = 200
for it in range(N):
    n = random.randint(2, 6)
    levels = random.sample(range(1, 30), n)
    k = random.randint(1, n - 1)
    data = f"{n} {k}\n" + "\n".join(map(str, levels)) + "\n"
    a = subprocess.run([MAIN], input=data, capture_output=True, text=True).stdout.split()
    b = subprocess.run([BRUTE], input=data, capture_output=True, text=True).stdout.split()
    if a != b:
        bad += 1
        print("MISMATCH", it, data, a, b)
        if bad > 3:
            sys.exit(1)
print("done", N, "cases, mismatches =", bad)
