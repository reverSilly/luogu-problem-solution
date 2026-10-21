#!/usr/bin/env python3
# P1855 对拍：二维费用背包 vs 子集枚举
import random
import subprocess
import sys
import time

STD_SRC = "/tmp/luogu-ps/appendix/P1855/P1855.cpp"
BRU_SRC = "/tmp/luogu-ps/appendix/P1855/P1855_brute.cpp"
STD = "/tmp/P1855"
BRU = "/tmp/P1855_brute"

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
        raise RuntimeError(path + " " + r.stderr.decode())
    return r.stdout.decode().strip()

def main():
    if compile_all():
        return 1
    rng = random.Random(20261021)
    cases = []
    cases.append("""6 10 10
1 1
2 3
3 2
2 5
5 2
4 3
""")
    cases.append("1 0 0\n0 0\n")
    cases.append("1 0 0\n1 0\n")
    cases.append("3 5 5\n0 0\n0 1\n6 1\n")
    cases.append("2 0 10\n0 3\n0 4\n")
    cases.append("4 200 200\n1 1\n2 2\n3 3\n4 4\n")
    for _ in range(70):
        n = rng.randint(1, 16)
        M = rng.randint(0, 40)
        T = rng.randint(0, 40)
        lines = [f"{n} {M} {T}"]
        for _i in range(n):
            # 偶尔费用为 0，偶尔超过容量
            if rng.random() < 0.15:
                c, t = 0, rng.randint(0, T)
            elif rng.random() < 0.1:
                c, t = rng.randint(M + 1, M + 5), rng.randint(0, T + 3)
            else:
                c = rng.randint(0, max(0, M))
                t = rng.randint(0, max(0, T))
            lines.append(f"{c} {t}")
        cases.append("\n".join(lines) + "\n")
    for idx, data in enumerate(cases):
        so = run(STD, data)
        bo = run(BRU, data)
        if so != bo:
            print("MISMATCH", idx)
            print(data)
            print("std", so, "brute", bo)
            return 1
    # 最大规模
    rng2 = random.Random(1)
    n, M, T = 100, 200, 200
    lines = [f"{n} {M} {T}"]
    for _ in range(n):
        lines.append(f"{rng2.randint(0, 200)} {rng2.randint(0, 200)}")
    big = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    so = run(STD, big)
    dt = time.perf_counter() - t0
    if not so.strip().lstrip("-").isdigit():
        print("BAD MAX", so)
        return 1
    print(f"max n=100 M=T=200 time={dt:.6f}s out={so}")
    print(f"{len(cases)} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
