#!/usr/bin/env python3
# P1870 对拍：闭式加速路程 vs 二分行驶时间。浮点绝对误差 1e-4
import random
import subprocess
import sys
import time

STD_SRC = "/tmp/luogu-ps/appendix/P1870/P1870.cpp"
BRU_SRC = "/tmp/luogu-ps/appendix/P1870/P1870_brute.cpp"
STD = "/tmp/P1870"
BRU = "/tmp/P1870_brute"

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

def close(a, b):
    if abs(a - b) <= 1e-4:
        return True
    scale = max(1.0, abs(a), abs(b))
    return abs(a - b) <= 1e-4 * scale

def check(so, bo):
    A = so.split()
    B = bo.split()
    if len(A) != len(B):
        return False
    for x, y in zip(A, B):
        if not close(float(x), float(y)):
            return False
    return True

def pack(n, a, d, buses):
    lines = [f"{n} {a} {d}"]
    for t, v in buses:
        lines.append(f"{t} {v}")
    return "\n".join(lines) + "\n"

def main():
    if compile_all():
        return 1
    rng = random.Random(20261021)
    cases = []
    cases.append(pack(3, 10, 10000, [(0, 10), (5, 11), (1000, 1)]))
    cases.append(pack(1, 2, 26, [(28, 29)]))
    cases.append(pack(1, 1, 1, [(0, 1)]))
    cases.append(pack(1, 10**6, 10**6, [(0, 10**6)]))
    cases.append(pack(2, 1, 10, [(0, 1), (1, 100)]))
    cases.append(pack(3, 5, 100, [(0, 10), (1, 10), (2, 10)]))
    expect0 = [1000.5, 1000.5, 11000.05]
    expect1 = [33.0990195136]
    fixed_expect = {0: expect0, 1: expect1}
    for _ in range(40):
        n = rng.randint(1, 30)
        a = rng.randint(1, 1000)
        d = rng.randint(1, 100000)
        t = 0
        buses = []
        for i in range(n):
            if i:
                t += rng.randint(1, 50)
            v = rng.randint(1, 1000)
            buses.append((t, v))
        cases.append(pack(n, a, d, buses))
    for idx, data in enumerate(cases):
        so = run(STD, data)
        bo = run(BRU, data)
        if not check(so, bo):
            print("MISMATCH", idx)
            print(data)
            print("std", so)
            print("brute", bo)
            return 1
        if idx in fixed_expect:
            got = list(map(float, so.split()))
            for g, e in zip(got, fixed_expect[idx]):
                if abs(g - e) > 1e-4 and abs(g - e) > 1e-4 * max(1.0, abs(e)):
                    print("SAMPLE", idx, got, e)
                    return 1
    # 最大规模，两边都算
    n = 100000
    a = 10**6
    d = 10**6
    buses = []
    t = 0
    rng2 = random.Random(19)
    for i in range(n):
        if i:
            t += rng2.randint(1, 5)
            if t > 10**6:
                t = 10**6 - (n - i)
                # 保证严格递增且不超过 1e6：重新构造
                break
        buses.append((t, rng2.randint(1, 10**6)))
    # 若中途 break，改用均匀时间
    if len(buses) != n:
        buses = []
        for i in range(n):
            buses.append((i * 10, rng2.randint(1, 10**6)))  # t=0..999990
    big = pack(n, a, d, buses)
    t0 = time.perf_counter()
    so = run(STD, big, timeout=30)
    dt = time.perf_counter() - t0
    bo = run(BRU, big, timeout=60)
    if not check(so, bo):
        print("MAX MISMATCH")
        print(so.splitlines()[:5], bo.splitlines()[:5])
        return 1
    print(f"max n=1e5 time={dt:.6f}s")
    print(f"{len(cases)+1} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
