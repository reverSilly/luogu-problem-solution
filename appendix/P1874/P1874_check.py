#!/usr/bin/env python3
# P1874 对拍：前缀和式 DP vs 带剪枝的分段 DFS
import os
import random
import subprocess
import sys
import tempfile
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.join(BASE, "P1874.cpp")
BRU = os.path.join(BASE, "P1874_brute.cpp")
EXE = os.path.join(tempfile.gettempdir(), "P1874")
EXEB = os.path.join(tempfile.gettempdir(), "P1874_brute")
CXX = ["g++", "-O2", "-std=c++17", "-Wall", "-Wextra"]

SAMPLES = [
("""99999
45
""", "4"),
("""12
3
""", "1"),
("""303
6
""", "1"),
("""1
2
""", "-1"),
("""01
1
""", "0"),
("""10
1
""", "1"),
("""000100
100
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


def gen(rng):
    ln = rng.randint(1, 14)
    s = "".join(str(rng.randint(0, 9)) for _ in range(ln))
    if rng.random() < 0.3:
        s = "0" * rng.randint(0, ln // 2) + s
        s = s[:ln]
        if not s:
            s = "0"
    tar = rng.randint(1, 2000 if ln <= 8 else 100000)
    return f"{s}\n{tar}\n"


def main():
    if build():
        return 1
    rng = random.Random(1874)
    cases = []
    for data, exp in SAMPLES:
        cases.append(data)
        a = run(EXE, data)
        b = run(EXEB, data)
        if a != exp or b != exp:
            print("样例失败", exp, a, b, data)
            return 1
    fixed = [
        "0\n1\n",
        "00\n1\n",
        "100\n1\n",
        "999\n27\n",
        "999\n9\n",
        "111111\n6\n",
        "111111\n5\n",
        "100000\n1\n",
        "000001\n1\n",
    ]
    for data in fixed:
        cases.append(data)
        a = run(EXE, data)
        b = run(EXEB, data)
        if a is None or b is None or a != b:
            print("MISMATCH fixed")
            print(data, a, b)
            return 1
    for _ in range(70):
        data = gen(rng)
        cases.append(data)
        a = run(EXE, data)
        b = run(EXEB, data)
        if a is None or b is None or a != b:
            print("MISMATCH")
            print(data)
            print("std:", a, "brute:", b)
            return 1
    s = "".join(str(random.Random(1).randint(0, 9)) for _ in range(40))
    data = s + "\n100000\n"
    t0 = time.perf_counter()
    a = run(EXE, data)
    dt = time.perf_counter() - t0
    if a is None:
        return 1
    # 长串边界：全 1 求和 40，必须拆成 40 个 1
    a2 = run(EXE, "1" * 40 + "\n40\n")
    if a2 != "39":
        print("长全 1 失败", a2)
        return 1
    a3 = run(EXE, "0" * 39 + "1\n1\n")
    if a3 != "0":
        print("前导零失败", a3)
        return 1
    a4 = run(EXE, "9" * 40 + "\n1\n")
    if a4 != "-1":
        print("不可能失败", a4)
        return 1
    print(f"MAX len=40 n=100000 time={dt:.4f}s")
    print(f"{len(cases)} tests OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
