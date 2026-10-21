#!/usr/bin/env python3
# P1854 对拍：标程区间 DP vs 暴力枚举花瓶组合
import random
import subprocess
import sys
import time

STD_SRC = "/tmp/luogu-ps/appendix/P1854/P1854.cpp"
BRU_SRC = "/tmp/luogu-ps/appendix/P1854/P1854_brute.cpp"
STD = "/tmp/P1854"
BRU = "/tmp/P1854_brute"

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

def run(path, data, timeout=20):
    r = subprocess.run([path], input=data.encode(), capture_output=True, timeout=timeout)
    if r.returncode != 0:
        raise RuntimeError(r.stderr.decode())
    return r.stdout.decode().strip()

def parse_case(data):
    lines = data.strip().splitlines()
    f, v = map(int, lines[0].split())
    a = []
    for i in range(f):
        a.append(list(map(int, lines[i + 1].split())))
    return f, v, a

def check_scheme(data, out, expect):
    lines = out.splitlines()
    if len(lines) != 2:
        return "行数不是 2: " + out
    val = int(lines[0])
    pos = list(map(int, lines[1].split())) if lines[1].strip() else []
    f, v, a = parse_case(data)
    if val != expect:
        return f"最优值 {val} != 暴力 {expect}"
    if len(pos) != f:
        return "方案长度不对"
    prev = 0
    s = 0
    for i, p in enumerate(pos):
        if not (prev < p <= v):
            return f"花瓶不严格递增或越界 {pos}"
        s += a[i][p - 1]
        prev = p
    if s != val:
        return f"方案美学值 {s} != {val}"
    return None

def main():
    if compile_all():
        return 1
    rng = random.Random(20261021)
    samples = []
    samples.append("""3 5
7 23 -5 -24 16
5 21 -4 10 23
-21 5 -4 -20 20
""")
    cases = list(samples)
    # 边界
    cases.append("1 1\n-50\n")
    cases.append("1 5\n-3 9 -1 4 0\n")
    cases.append("2 2\n-50 -50\n-50 -50\n")
    cases.append("3 3\n1 2 3\n4 5 6\n7 8 9\n")
    cases.append("4 4\n50 50 50 50\n50 50 50 50\n50 50 50 50\n50 50 50 50\n")
    for _ in range(80):
        f = rng.randint(1, 7)
        v = rng.randint(f, 12)
        rows = []
        for _i in range(f):
            rows.append(" ".join(str(rng.randint(-50, 50)) for _ in range(v)))
        cases.append(f"{f} {v}\n" + "\n".join(rows) + "\n")
    for idx, data in enumerate(cases):
        try:
            so = run(STD, data)
            bo = run(BRU, data)
        except Exception as e:
            print("RUNTIME", idx, e)
            print(data)
            return 1
        expect = int(bo.strip())
        err = check_scheme(data, so, expect)
        if err:
            print("MISMATCH", idx, err)
            print(data)
            print("std:", so)
            print("brute:", bo)
            return 1
    # 最大范围计时，只跑标程并检查方案自洽
    rng2 = random.Random(7)
    f = v = 100
    rows = []
    for _i in range(f):
        rows.append(" ".join(str(rng2.randint(-50, 50)) for _ in range(v)))
    big = f"{f} {v}\n" + "\n".join(rows) + "\n"
    t0 = time.perf_counter()
    so = run(STD, big, timeout=10)
    dt = time.perf_counter() - t0
    lines = so.splitlines()
    pos = list(map(int, lines[1].split()))
    _, _, a = parse_case(big)
    if len(pos) != 100 or any(not (pos[i] < pos[i + 1]) for i in range(99)):
        print("BIG SCHEME BAD", so[:200])
        return 1
    s = sum(a[i][pos[i] - 1] for i in range(100))
    if s != int(lines[0]):
        print("BIG SUM BAD")
        return 1
    print(f"max F=V=100 time={dt:.6f}s")
    print(f"{len(cases)} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
