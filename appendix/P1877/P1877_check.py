#!/usr/bin/env python3
# P1877 对拍：按歌曲顺序的可达性 DP vs 加减 DFS
import os
import random
import subprocess
import sys
import tempfile
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.join(BASE, "P1877.cpp")
BRU = os.path.join(BASE, "P1877_brute.cpp")
EXE = os.path.join(tempfile.gettempdir(), "P1877")
EXEB = os.path.join(tempfile.gettempdir(), "P1877_brute")
CXX = ["g++", "-O2", "-std=c++17", "-Wall", "-Wextra"]

SAMPLES = [
("""3 5 10
5 3 7
""", "10"),
("""1 0 3
5
""", "-1"),
("""1 0 10
5
""", "5"),
("""2 5 10
1 1
""", "7"),
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
    rng = random.Random(1877)
    cases = []
    for data, exp in SAMPLES:
        cases.append(data)
        a = run(EXE, data)
        b = run(EXEB, data)
        if a != exp or b != exp:
            print("样例失败", exp, a, b)
            print(data)
            return 1
    for _ in range(120):
        cap = rng.randint(1, 40)
        n = rng.randint(1, 18)
        beg = rng.randint(0, cap)
        cs = [rng.randint(1, cap) for _ in range(n)]
        data = f"{n} {beg} {cap}\n" + " ".join(map(str, cs)) + "\n"
        cases.append(data)
        a = run(EXE, data)
        b = run(EXEB, data)
        if a is None or b is None or a != b:
            print("MISMATCH")
            print(data)
            print("std:", a, "brute:", b)
            return 1
    cap = 1000
    n = 50
    beg = 500
    cs = [rng.randint(1, cap) for _ in range(n)]
    data = f"{n} {beg} {cap}\n" + " ".join(map(str, cs)) + "\n"
    t0 = time.perf_counter()
    a = run(EXE, data)
    dt = time.perf_counter() - t0
    if a is None:
        return 1
    # 满规模再跑一遍 DP 自己不能当证明；用滚动集合在 python 里独立算
    reach = {beg}
    for c in cs:
        nxt = set()
        for v in reach:
            if v + c <= cap:
                nxt.add(v + c)
            if v - c >= 0:
                nxt.add(v - c)
        reach = nxt
    exp = str(max(reach) if reach else -1)
    if a != exp:
        print("大样例不符", a, exp)
        return 1
    print(f"MAX n=50 cap=1000 time={dt:.4f}s")
    print(f"{len(cases)} tests OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
