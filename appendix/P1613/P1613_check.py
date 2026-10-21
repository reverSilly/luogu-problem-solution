#!/usr/bin/env python3
# P1613 对拍：随机小有向图（含自环、重边、保证 1 可达 n），
# 比较标程（倍增预处理 2^k 步 + Floyd）与暴力（枚举所有可达行走长度 L<=2^18，答案 = min popcount(L)）
import random, subprocess, sys
from collections import deque

SOL = "/tmp/P1613"
BRU = "/tmp/P1613_brute"

def gen(rng):
    n = rng.randint(2, 5)
    edges = []
    # 先铺一条 1->2->...->n 的链, 再随机添边, 保证可达
    for i in range(1, n):
        edges.append((i, i + 1))
    extra = rng.randint(0, 5)
    for _ in range(extra):
        edges.append((rng.randint(1, n), rng.randint(1, n)))
    rng.shuffle(edges)
    lines = [f"{n} {len(edges)}"] + [f"{u} {v}" for u, v in edges]
    return "\n".join(lines) + "\n"

def run(exe, data):
    p = subprocess.run([exe], input=data, capture_output=True, text=True)
    return p.stdout.strip()

def main():
    iters = int(sys.argv[1]) if len(sys.argv) > 1 else 200
    rng = random.Random(97531)
    cmp_n = 0
    for t in range(iters):
        data = gen(rng)
        a, b = run(SOL, data), run(BRU, data)
        if b == "-1":                    # 暴力上界内没走到终点, 不作比较
            continue
        if a != b:
            print("MISMATCH at", t)
            print(data)
            print("sol =", a, "brute =", b)
            return
        cmp_n += 1
    print(f"done {iters} generated, {cmp_n} compared, mismatches = 0")

main()
