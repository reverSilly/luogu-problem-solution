#!/usr/bin/env python3
# P1869 对拍：按位子集 vs 杨辉三角模 2 / Kummer 幂次
import random
import subprocess
import sys
import time

STD_SRC = "/tmp/luogu-ps/appendix/P1869/P1869.cpp"
BRU_SRC = "/tmp/luogu-ps/appendix/P1869/P1869_brute.cpp"
STD = "/tmp/P1869"
BRU = "/tmp/P1869_brute"

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

def comb_odd(n, k):
    if k < 0 or k > n:
        return 0
    k = min(k, n - k)
    res = 1
    for i in range(k):
        res = res * (n - i) // (i + 1)
    return res & 1

def main():
    if compile_all():
        return 1
    rng = random.Random(20261021)
    cases = []
    cases.append("3\n1 1\n1 0\n2 1\n")
    # 手工：C(5,2)=10 偶，C(6,2)=15 奇，C(6,1)=6 偶，C(7,3)=35 奇，C(10,3)=120 偶
    cases.append("6\n5 2\n6 2\n6 1\n7 3\n10 3\n8 0\n")
    qs = []
    for n in range(0, 30):
        for k in range(0, n + 1):
            qs.append((n, k))
    # 打乱并拆成若干组，避免单次输入过大也无妨
    rng.shuffle(qs)
    chunk = []
    for n, k in qs:
        chunk.append(f"{n} {k}")
        if len(chunk) == 40:
            cases.append(str(len(chunk)) + "\n" + "\n".join(chunk) + "\n")
            chunk = []
    if chunk:
        cases.append(str(len(chunk)) + "\n" + "\n".join(chunk) + "\n")
    for _ in range(15):
        t = rng.randint(1, 30)
        lines = [str(t)]
        pairs = []
        for _i in range(t):
            n = rng.randint(0, 400)
            k = rng.randint(0, n)
            lines.append(f"{n} {k}")
            pairs.append((n, k))
        cases.append(("\n".join(lines) + "\n", pairs))
    # 规范化：前几项是纯字符串
    norm = []
    oracles = []
    for c in cases:
        if isinstance(c, tuple):
            norm.append(c[0])
            oracles.append(c[1])
        else:
            norm.append(c)
            # 从文本抽出
            lines = c.strip().splitlines()
            t = int(lines[0])
            pairs = []
            for i in range(t):
                n, k = map(int, lines[i + 1].split())
                pairs.append((n, k))
            oracles.append(pairs)
    for idx, data in enumerate(norm):
        so = run(STD, data)
        bo = run(BRU, data)
        if so != bo:
            print("MISMATCH std/brute", idx)
            print(data[:400])
            print("std", so)
            print("brute", bo)
            return 1
        outs = list(map(int, so.split()))
        pairs = oracles[idx]
        if len(outs) != len(pairs):
            print("LEN", idx)
            return 1
        for (n, k), bit in zip(pairs, outs):
            if n <= 500:
                o = comb_odd(n, k)
                if o != bit:
                    print("ORACLE", n, k, "got", bit, "want", o)
                    print(data[:300])
                    return 1
    # 大 N：杨辉表走 Kummer 分支
    lines = ["5"]
    big_pairs = [(10**5, 0), (10**5, 10**5), (10**5, 1), (99999, 3), (2**16, 2**15)]
    for n, k in big_pairs:
        lines.append(f"{n} {k}")
    data = "\n".join(lines) + "\n"
    so = run(STD, data)
    bo = run(BRU, data)
    if so != bo:
        print("BIG MIS", so, bo)
        return 1
    # 最大规模计时
    rng2 = random.Random(17)
    t = 100000
    buf = [str(t)]
    for _ in range(t):
        n = rng2.randint(0, 10**5)
        k = rng2.randint(0, n)
        buf.append(f"{n} {k}")
    big = "\n".join(buf) + "\n"
    t0 = time.perf_counter()
    so = run(STD, big)
    dt = time.perf_counter() - t0
    if len(so.split()) != t:
        print("BAD COUNT")
        return 1
    print(f"max t=1e5 N<=1e5 time={dt:.6f}s")
    print(f"{len(norm)+1} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
