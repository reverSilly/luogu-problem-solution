#!/usr/bin/env python3
# P1883 对拍：三分 vs 顶点/交点候选。按绝对误差比较，不要求字符串全等。
import os
import random
import subprocess
import sys
import tempfile
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.join(BASE, "P1883.cpp")
BRU = os.path.join(BASE, "P1883_brute.cpp")
EXE = os.path.join(tempfile.gettempdir(), "P1883")
EXEB = os.path.join(tempfile.gettempdir(), "P1883_brute")
CXX = ["g++", "-O2", "-std=c++17", "-Wall", "-Wextra"]
EPS = 5e-5

SAMPLES = [
("""2
1
2 0 0
2
2 0 0
2 -4 2
""", [0.0, 0.5]),
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


def parse_vals(text):
    if text is None or text == "":
        return None
    try:
        return [float(x) for x in text.split()]
    except ValueError:
        return None


def close(a, b):
    if len(a) != len(b):
        return False
    return all(abs(x - y) <= EPS for x, y in zip(a, b))


def gen(rng, n):
    lines = ["1", str(n)]
    for _ in range(n):
        aa = rng.randint(0, 100)
        bb = rng.randint(-5000, 5000)
        cc = rng.randint(-5000, 5000)
        lines.append(f"{aa} {bb} {cc}")
    return "\n".join(lines) + "\n"


def main():
    if build():
        return 1
    rng = random.Random(1883)
    cases = []
    for data, exp in SAMPLES:
        cases.append(data)
        a = parse_vals(run(EXE, data))
        b = parse_vals(run(EXEB, data))
        if a is None or b is None or not close(a, exp) or not close(b, exp):
            print("样例失败", a, b, exp)
            return 1
    for _ in range(30):
        cases.append(gen(rng, rng.randint(1, 25)))
    for _ in range(8):
        cases.append(gen(rng, rng.randint(26, 60)))
    # 退化成一次、常数
    cases.append("1\n3\n0 0 5\n0 -3 10\n0 4 -1\n")
    cases.append("1\n1\n0 0 -7\n")
    for i, data in enumerate(cases):
        if i < len(SAMPLES):
            continue
        a = parse_vals(run(EXE, data))
        b = parse_vals(run(EXEB, data))
        if a is None or b is None or not close(a, b):
            print("MISMATCH", i)
            print(data[:400])
            print("std:", a, "brute:", b)
            return 1
    data = gen(random.Random(99), 10000)
    # T=3 组满规模
    blocks = ["3"]
    rr = random.Random(100)
    for _ in range(3):
        blocks.append("10000")
        for _j in range(10000):
            aa = rr.randint(0, 100)
            bb = rr.randint(-5000, 5000)
            cc = rr.randint(-5000, 5000)
            blocks.append(f"{aa} {bb} {cc}")
    data = "\n".join(blocks) + "\n"
    t0 = time.perf_counter()
    a = run(EXE, data)
    dt = time.perf_counter() - t0
    vals = parse_vals(a)
    if vals is None or len(vals) != 3:
        print("大样例输出失败", a)
        return 1
    print(f"MAX T=3 n=10000 time={dt:.4f}s")
    print(f"{len(cases)} tests OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
