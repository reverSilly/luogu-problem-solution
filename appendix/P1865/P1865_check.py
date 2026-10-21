#!/usr/bin/env python3
# P1865 对拍：线性筛前缀和 vs 试除
import random
import subprocess
import sys
import time

STD_SRC = "/tmp/luogu-ps/appendix/P1865/P1865.cpp"
BRU_SRC = "/tmp/luogu-ps/appendix/P1865/P1865_brute.cpp"
STD = "/tmp/P1865"
BRU = "/tmp/P1865_brute"

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

def main():
    if compile_all():
        return 1
    rng = random.Random(20261021)
    cases = []
    cases.append("2 5\n1 3\n2 6\n")
    cases.append("1 1\n1 1\n")
    cases.append("3 10\n1 10\n2 2\n-5 3\n")
    cases.append("4 10\n1 1\n1 2\n8 10\n0 10\n")
    cases.append("2 2\n1 2\n2 2\n")
    for _ in range(30):
        m = rng.randint(1, 1500)
        q = rng.randint(1, 25)
        lines = [f"{q} {m}"]
        for _i in range(q):
            kind = rng.randrange(5)
            if kind == 0:
                l = rng.randint(-10**9, 0)
                r = rng.randint(l, m)
            elif kind == 1:
                l = rng.randint(1, m)
                r = rng.randint(m + 1, 10**9)
            else:
                l = rng.randint(1, m)
                r = rng.randint(l, m)
            lines.append(f"{l} {r}")
        cases.append("\n".join(lines) + "\n")
    for idx, data in enumerate(cases):
        so = run(STD, data)
        bo = run(BRU, data)
        if so != bo:
            print("MISMATCH", idx)
            print(data)
            print("std", so)
            print("brute", bo)
            return 1
    rng2 = random.Random(11)
    m = 1000000
    q = 1000
    lines = [f"{q} {m}"]
    for _ in range(q):
        if rng2.randrange(10) == 0:
            lines.append(f"{rng2.randint(-10**9, 0)} {rng2.randint(1, 10**9)}")
        else:
            l = rng2.randint(1, m)
            r = rng2.randint(l, m)
            lines.append(f"{l} {r}")
    big = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    so = run(STD, big)
    dt = time.perf_counter() - t0
    if so.count("\n") + 1 != q:
        print("BAD LINES", so.count("\n"))
        return 1
    print(f"max m=1e6 n=1000 time={dt:.6f}s")
    print(f"{len(cases)} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
