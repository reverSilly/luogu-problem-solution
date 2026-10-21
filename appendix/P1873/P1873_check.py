#!/usr/bin/env python3
# P1873 对拍：二分高度 vs 从高到低线性枚举
import os
import random
import subprocess
import sys
import tempfile
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.join(BASE, "P1873.cpp")
BRU = os.path.join(BASE, "P1873_brute.cpp")
EXE = os.path.join(tempfile.gettempdir(), "P1873")
EXEB = os.path.join(tempfile.gettempdir(), "P1873_brute")
CXX = ["g++", "-O2", "-std=c++17", "-Wall", "-Wextra"]

SAMPLES = [
("""4 7
20 15 10 17
""", "15"),
("""5 20
4 42 40 26 46
""", "36"),
]


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


def main():
    if build():
        return 1
    rng = random.Random(1873)
    cases = []
    for data, exp in SAMPLES:
        cases.append(data)
        a = run(EXE, data)
        b = run(EXEB, data)
        if a != exp or b != exp:
            print("样例失败", exp, a, b)
            print(data)
            return 1
    for _ in range(80):
        n = rng.randint(1, 300)
        mxh = rng.randint(1, 2500)
        hs = [rng.randint(0, mxh) for _ in range(n)]
        tot = sum(hs)
        if tot < 1:
            hs[0] = 1
            tot = 1
        m = rng.randint(1, tot)
        data = f"{n} {m}\n" + " ".join(map(str, hs)) + "\n"
        cases.append(data)
        a = run(EXE, data)
        b = run(EXEB, data)
        if a is None or b is None or a != b:
            print("MISMATCH")
            print(data)
            print("std:", a, "brute:", b)
            return 1
    rng3 = random.Random(9)
    n = 1000000
    hs = [rng3.randint(0, 400000) for _ in range(n)]
    m = rng3.randint(1, 2000000000)
    data = f"{n} {m}\n" + " ".join(map(str, hs)) + "\n"
    t0 = time.perf_counter()
    a = run(EXE, data)
    dt = time.perf_counter() - t0
    if a is None:
        return 1
    # python 二分独立验算
    lo, hi = 0, max(hs)
    def cut(H):
        return sum(h - H for h in hs if h > H)
    while lo < hi:
        mid = (lo + hi + 1) // 2
        if cut(mid) >= m:
            lo = mid
        else:
            hi = mid - 1
    if a != str(lo):
        print("大样例与 python 二分不符", a, lo)
        return 1
    print(f"MAX n=1000000 time={dt:.4f}s")
    print(f"{len(cases)} tests OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
