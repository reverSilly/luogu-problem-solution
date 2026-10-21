#!/usr/bin/env python3
# P1881 对拍：倍长坐标对称校验 vs 差分回文计数
import os
import random
import subprocess
import sys
import tempfile
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.join(BASE, "P1881.cpp")
BRU = os.path.join(BASE, "P1881_brute.cpp")
EXE = os.path.join(tempfile.gettempdir(), "P1881")
EXEB = os.path.join(tempfile.gettempdir(), "P1881_brute")
CXX = ["g++", "-O2", "-std=c++17", "-Wall", "-Wextra"]

SAMPLES = [
("""5 10
0
10
6
2
4
""", "4"),
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


def gen(rng):
    L = rng.randint(1, 10000)
    n = rng.randint(2, min(100, L + 1))
    xs = {0, L}
    while len(xs) < n:
        xs.add(rng.randint(0, L))
    arr = list(xs)
    rng.shuffle(arr)
    lines = [f"{len(arr)} {L}"] + [str(x) for x in arr]
    return "\n".join(lines) + "\n"


def main():
    if build():
        return 1
    rng = random.Random(1881)
    cases = []
    for data, exp in SAMPLES:
        cases.append(data)
        a = run(EXE, data)
        b = run(EXEB, data)
        if a != exp or b != exp:
            print("样例失败", exp, a, b)
            return 1
    cases.append("2 1\n0\n1\n")
    cases.append("2 10\n0\n10\n")
    cases.append("3 4\n0\n4\n2\n")
    for _ in range(80):
        cases.append(gen(rng))
    for i, data in enumerate(cases):
        if i < len(SAMPLES):
            continue
        a = run(EXE, data)
        b = run(EXEB, data)
        if a is None or b is None or a != b:
            print("MISMATCH", i)
            print(data)
            print("std:", a, "brute:", b)
            return 1
    # 最大范围：L=10000, N=100
    data = gen(random.Random(10000))
    # 强制最大
    L = 10000
    xs = [0, L] + random.Random(3).sample(range(1, L), 98)
    data = f"100 {L}\n" + "\n".join(map(str, xs)) + "\n"
    t0 = time.perf_counter()
    a = run(EXE, data)
    dt = time.perf_counter() - t0
    b = run(EXEB, data)
    if a is None or a != b:
        print("最大范围不符", a, b)
        return 1
    print(f"MAX N=100 L=10000 time={dt:.4f}s")
    print(f"{len(cases)} tests OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
