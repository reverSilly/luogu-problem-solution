#!/usr/bin/env python3
# P1876 对拍：完全平方数枚举 vs 按倍数翻转计数
import os
import random
import subprocess
import sys
import tempfile
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.join(BASE, "P1876.cpp")
BRU = os.path.join(BASE, "P1876_brute.cpp")
EXE = os.path.join(tempfile.gettempdir(), "P1876")
EXEB = os.path.join(tempfile.gettempdir(), "P1876_brute")
CXX = ["g++", "-O2", "-std=c++17", "-Wall", "-Wextra"]


def build():
    for src, out in ((SRC, EXE), (BRU, EXEB)):
        r = subprocess.run(CXX + [src, "-o", out], capture_output=True, text=True)
        if r.returncode != 0 or r.stderr.strip():
            print("编译失败或有警告:", src)
            print(r.stderr)
            return 1
    return 0


def run(exe, data):
    r = subprocess.run([exe], input=data.encode(), capture_output=True)
    if r.returncode != 0:
        print("运行失败", exe, r.stderr.decode())
        return None
    return r.stdout.decode().strip()


def squares(n):
    ans = []
    i = 1
    while i <= n // i:
        ans.append(str(i * i))
        i += 1
    return " ".join(ans)


def main():
    if build():
        return 1
    rng = random.Random(1876)
    cases = [1, 2, 3, 4, 5, 9, 10, 15, 16, 100, 99991]
    exp5 = run(EXE, "5\n")
    if exp5 != "1 4":
        print("样例失败", exp5)
        return 1
    brut5 = run(EXEB, "5\n")
    if brut5 != "1 4":
        print("暴力样例失败", brut5)
        return 1
    for _ in range(60):
        cases.append(rng.randint(1, 80000))
    for n in cases:
        data = f"{n}\n"
        a = run(EXE, data)
        b = run(EXEB, data)
        if a is None or b is None or a != b:
            print("MISMATCH", n)
            print("std:", a)
            print("brute:", b)
            return 1
    n = 1 << 40
    data = f"{n}\n"
    t0 = time.perf_counter()
    a = run(EXE, data)
    dt = time.perf_counter() - t0
    if a != squares(n):
        print("2^40 与平方枚举不符")
        return 1
    print(f"MAX n=2^40 time={dt:.4f}s")
    print(f"{len(cases)} tests OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
