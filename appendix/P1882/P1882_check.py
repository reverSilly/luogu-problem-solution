#!/usr/bin/env python3
# P1882 对拍：Dijkstra（边权=通知者用时） vs 按完成时间扫描事件
import os
import random
import subprocess
import sys
import tempfile
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.join(BASE, "P1882.cpp")
BRU = os.path.join(BASE, "P1882_brute.cpp")
EXE = os.path.join(tempfile.gettempdir(), "P1882")
EXEB = os.path.join(tempfile.gettempdir(), "P1882_brute")
CXX = ["g++", "-O2", "-std=c++17", "-Wall", "-Wextra"]

SAMPLES = [
("""5
4 2 2 4
3 3 1 3 4
7 1 5
4 2 3 5
1 0
""", "14"),
("""1
5 0
""", "5"),
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


def gen(rng, n, dense=False):
    lines = [str(n)]
    for i in range(n):
        L = rng.randint(1, 1000)
        if dense:
            m = rng.randint(0, n)
        else:
            m = rng.randint(0, min(n, 8))
        vs = [rng.randint(1, n) for _ in range(m)]
        lines.append(" ".join(map(str, [L, m] + vs)))
    return "\n".join(lines) + "\n"


def main():
    if build():
        return 1
    rng = random.Random(1882)
    cases = []
    for data, exp in SAMPLES:
        cases.append(data)
        a = run(EXE, data)
        b = run(EXEB, data)
        if a != exp or b != exp:
            print("样例失败", exp, a, b)
            print(data)
            return 1
    for _ in range(60):
        cases.append(gen(rng, rng.randint(1, 40)))
    for _ in range(20):
        cases.append(gen(rng, rng.randint(41, 200), dense=True))
    for i, data in enumerate(cases):
        if i < len(SAMPLES):
            continue
        a = run(EXE, data)
        b = run(EXEB, data)
        if a is None or b is None or a != b:
            print("MISMATCH", i)
            print(data[:500])
            print("std:", a, "brute:", b)
            return 1
    data = gen(random.Random(11), 1000, dense=False)
    # 每头牛通知最多 30 头，边数约 3e4
    lines = ["1000"]
    rr = random.Random(12)
    for i in range(1000):
        L = rr.randint(1, 1000)
        m = rr.randint(0, 30)
        vs = [rr.randint(1, 1000) for _ in range(m)]
        lines.append(" ".join(map(str, [L, m] + vs)))
    data = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    a = run(EXE, data)
    dt = time.perf_counter() - t0
    b = run(EXEB, data)
    if a is None or a != b:
        print("最大范围不符", a, b)
        return 1
    print(f"MAX n=1000 time={dt:.4f}s")
    print(f"{len(cases)} tests OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
