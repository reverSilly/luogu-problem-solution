#!/usr/bin/env python3
# P1866 对拍：排序后乘法原理 vs 回溯计数
import random
import subprocess
import sys
import time

STD_SRC = "/tmp/luogu-ps/appendix/P1866/P1866.cpp"
BRU_SRC = "/tmp/luogu-ps/appendix/P1866/P1866_brute.cpp"
STD = "/tmp/P1866"
BRU = "/tmp/P1866_brute"
MOD = 1000000007

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

def main():
    if compile_all():
        return 1
    rng = random.Random(20261021)
    cases = []
    cases.append("2\n5 8\n")
    cases.append("1\n1\n")
    cases.append("1\n1000\n")
    cases.append("3\n1 1 1\n")
    cases.append("3\n3 3 3\n")
    cases.append("4\n2 1 3 4\n")
    cases.append("5\n5 1 4 2 3\n")
    for _ in range(50):
        n = rng.randint(1, 7)
        hi = rng.randint(1, 9)
        arr = [rng.randint(1, hi) for _ in range(n)]
        cases.append(str(n) + "\n" + " ".join(map(str, arr)) + "\n")
    for idx, data in enumerate(cases):
        so = run(STD, data)
        bo = run(BRU, data)
        if so != bo:
            print("MISMATCH", idx)
            print(data)
            print("std", so, "brute", bo)
            return 1
    rng2 = random.Random(13)
    n = 50
    arr = [rng2.randint(1, 1000) for _ in range(n)]
    big = str(n) + "\n" + " ".join(map(str, arr)) + "\n"
    t0 = time.perf_counter()
    so = run(STD, big)
    dt = time.perf_counter() - t0
    # 独立按同一公式再算一遍只为确认输出在模数内；正确性由小数据回溯保证
    b = sorted(arr)
    ans = 1
    for i, x in enumerate(b):
        ways = x - i
        if ways <= 0:
            ans = 0
            break
        ans = ans * ways % MOD
    if int(so) != ans:
        print("MAX FORMULA", so, ans)
        return 1
    print(f"max n=50 time={dt:.6f}s out={so}")
    print(f"{len(cases)} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
