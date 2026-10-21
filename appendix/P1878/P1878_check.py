#!/usr/bin/env python3
# P1878 对拍：链表+有序集合 vs 每步线性扫描相邻异性
import os
import random
import subprocess
import sys
import tempfile
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.join(BASE, "P1878.cpp")
BRU = os.path.join(BASE, "P1878_brute.cpp")
EXE = os.path.join(tempfile.gettempdir(), "P1878")
EXEB = os.path.join(tempfile.gettempdir(), "P1878_brute")
CXX = ["g++", "-O2", "-std=c++17", "-Wall", "-Wextra"]

SAMPLES = [
("""4
BGBG
4 2 4 3
""", """2
3 4
1 2"""),
("""4
BBGG
4 6 1 5
""", """2
2 3
1 4"""),
("""4
BGBB
1 1 2 3
""", """1
1 2"""),
("""1
B
5
""", "0"),
("""3
BBB
1 2 3
""", "0"),
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


def gen(rng, n):
    s = "".join(rng.choice("BG") for _ in range(n))
    a = [rng.randint(0, 10000000) for _ in range(n)]
    return f"{n}\n{s}\n" + " ".join(map(str, a)) + "\n"


def main():
    if build():
        return 1
    rng = random.Random(1878)
    cases = []
    for data, exp in SAMPLES:
        cases.append(data)
        a = run(EXE, data)
        b = run(EXEB, data)
        if a != exp or b != exp:
            print("样例失败")
            print(data)
            print("expect:", exp)
            print("std:", a)
            print("brute:", b)
            return 1
    for _ in range(40):
        data = gen(rng, rng.randint(1, 30))
        cases.append(data)
    for _ in range(25):
        data = gen(rng, rng.randint(31, 400))
        cases.append(data)
    for i, data in enumerate(cases):
        if i < len(SAMPLES):
            continue
        a = run(EXE, data)
        b = run(EXEB, data)
        if a is None or b is None or a != b:
            print("MISMATCH", i)
            print(data)
            print("std:", a)
            print("brute:", b)
            return 1
    data = gen(random.Random(7), 200000)
    t0 = time.perf_counter()
    a = run(EXE, data)
    dt = time.perf_counter() - t0
    if a is None:
        return 1
    lines = a.splitlines()
    k = int(lines[0])
    if k < 0 or k > 100000 or len(lines) != k + 1:
        print("大样例输出形态不对", k, len(lines))
        return 1
    print(f"MAX n=200000 time={dt:.4f}s")
    print(f"{len(cases)} tests OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
