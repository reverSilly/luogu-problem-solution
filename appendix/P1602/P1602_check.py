#!/usr/bin/env python3
# P1602 对拍: 标程(余数 BFS) vs 暴力(按数值递增枚举)
# 编译:
#   g++ -O2 -std=c++17 -Wall -o /tmp/P1602 P1602.cpp
#   g++ -O2 -std=c++17 -Wall -o /tmp/P1602_brute P1602_brute.cpp
import random
import subprocess
import sys

STD = "/tmp/P1602"
BRU = "/tmp/P1602_brute"


def run(path, data):
    r = subprocess.run([path], input=data.encode(), capture_output=True)
    return r.stdout.decode().strip()


def main():
    rng = random.Random(20260521)
    cases = [(k, m) for k in range(2, 11) for m in range(1, 11)]
    cases += [(2, 1), (10, 1), (2, 27), (10, 999), (9, 1000), (3, 999)]
    for _ in range(150):
        cases.append((rng.randint(2, 10), rng.randint(1, 30)))
    ok = 0
    for t, (k, m) in enumerate(cases):
        data = f"{k} {m}\n"
        a = run(STD, data)
        b = run(BRU, data)
        if b == "TOOBIG":
            continue                              # 暴力枚举规模不足, 跳过
        if a != b:
            print("MISMATCH on test", t, k, m)
            print("std:", a, "brute:", b)
            return 1
        if int(a) % m != 0 or any(int(c) >= k for c in a):
            print("INVALID answer on test", t, k, m, a)
            return 1
        ok += 1
    print(f"{ok} tests OK ({len(cases) - ok} skipped as TOOBIG)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
