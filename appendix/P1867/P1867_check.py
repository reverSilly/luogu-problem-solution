#!/usr/bin/env python3
# P1867 对拍：逐级扣经验 vs floor(log2(exp+1))
import random
import subprocess
import sys
import time

STD_SRC = "/tmp/luogu-ps/appendix/P1867/P1867.cpp"
BRU_SRC = "/tmp/luogu-ps/appendix/P1867/P1867_brute.cpp"
STD = "/tmp/P1867"
BRU = "/tmp/P1867_brute"

def compile_all():
    for src, out in ((STD_SRC, STD), (BRU_SRC, BRU)):
        r = subprocess.run(
            ["g++", "-O2", "-std=c++17", "-Wall", "-Wextra", "-o", out, src],
            capture_output=True, text=True)
        if r.returncode != 0 or r.stderr.strip():
            print("COMPILE FAIL", src)
            print(r.stderr)
            return 1
    return 0

def run(path, data, timeout=10):
    r = subprocess.run([path], input=data.encode(), capture_output=True, timeout=timeout)
    if r.returncode != 0:
        raise RuntimeError(r.stderr.decode())
    return r.stdout.decode().strip()

def main():
    if compile_all():
        return 1
    rng = random.Random(20261021)
    cases = []
    cases.append("2\n5.5 15\n4.5 24\n")
    cases.append("1\n10 100\n")
    cases.append("1\n-1 1\n")
    cases.append("1\n0 2\n")
    cases.append("3\n1 1\n1 1\n100 5\n")
    cases.append("2\n-5 0\n3.5 39\n")
    cases.append("4\n9 1\n-100 1\n0 0\n10 7\n")
    cases.append("1\n9.9 0\n")
    cases.append("2\n0.5 8\n9.5 100\n")
    for _ in range(80):
        n = rng.randint(1, 20)
        lines = [str(n)]
        for _i in range(n):
            # x 可正可负可小数；经验非负整数
            x = rng.randint(-20, 20) + rng.choice([0, 0, 0.5, -0.5, 0.25])
            a = rng.randint(0, 5000)
            lines.append(f"{x} {a}")
        cases.append("\n".join(lines) + "\n")
    # 总经验刚好是 2^L-1 与 2^L
    cases.append("1\n0 0\n")
    cases.append("1\n0 1\n")
    cases.append("1\n0 2\n")
    cases.append("1\n0 3\n")
    cases.append("1\n0 31\n")
    cases.append("1\n0 32\n")
    for idx, data in enumerate(cases):
        so = run(STD, data)
        bo = run(BRU, data)
        if so != bo:
            print("MISMATCH", idx)
            print(data)
            print("std", so, "brute", bo)
            return 1
    # 单独计时一次 n=20
    t0 = time.perf_counter()
    run(STD, cases[9])
    dt = time.perf_counter() - t0
    print(f"max n<=20 time={dt:.6f}s")
    print(f"{len(cases)} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
