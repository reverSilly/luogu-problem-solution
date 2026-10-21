#!/usr/bin/env python3
# P1862 对拍：中位数闭式 vs 枚举每一条水平线
import random
import subprocess
import sys
import time

STD_SRC = "/tmp/luogu-ps/appendix/P1862/P1862.cpp"
BRU_SRC = "/tmp/luogu-ps/appendix/P1862/P1862_brute.cpp"
STD = "/tmp/P1862"
BRU = "/tmp/P1862_brute"

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

def run(path, data, timeout=30):
    r = subprocess.run([path], input=data.encode(), capture_output=True, timeout=timeout)
    if r.returncode != 0:
        raise RuntimeError(r.stderr.decode())
    return r.stdout.decode().strip()

def pack(pts):
    return str(len(pts)) + "\n" + "\n".join(f"{x} {y}" for x, y in pts) + "\n"

def main():
    if compile_all():
        return 1
    rng = random.Random(20261021)
    cases = []
    cases.append(pack([(1, 2), (2, 2), (1, 3), (3, -2), (3, 3)]))
    cases.append(pack([(0, 0)]))
    cases.append(pack([(1, -10000), (2, 10000)]))
    cases.append(pack([(i, 5) for i in range(10)]))
    cases.append(pack([(0, -3), (0, -1), (0, 4), (0, 8)]))
    for _ in range(40):
        n = rng.randint(1, 80)
        pts = [(rng.randint(-10000, 10000), rng.randint(-30, 30)) for _ in range(n)]
        cases.append(pack(pts))
    for _ in range(8):
        n = rng.randint(1, 200)
        pts = [(rng.randint(-10000, 10000), rng.randint(-200, 200)) for _ in range(n)]
        cases.append(pack(pts))
    for idx, data in enumerate(cases):
        so = run(STD, data)
        bo = run(BRU, data)
        if so != bo:
            print("MISMATCH", idx)
            print(data[:500])
            print("std", so, "brute", bo)
            return 1
    rng2 = random.Random(9)
    pts = [(rng2.randint(-10000, 10000), rng2.randint(-10000, 10000)) for _ in range(10000)]
    big = pack(pts)
    t0 = time.perf_counter()
    so = run(STD, big)
    dt = time.perf_counter() - t0
    if not so.lstrip("-").isdigit():
        print("BAD", so)
        return 1
    print(f"max n=10000 time={dt:.6f}s out={so}")
    print(f"{len(cases)} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
