#!/usr/bin/env python3
# P1610 对拍：随机小规模灯位（含大空隙、相邻灯），比较标程（贪心）与暴力（枚举保留集合）
import random, subprocess, sys

SOL = "/tmp/P1610"
BRU = "/tmp/P1610_brute"

def gen(rng):
    n = rng.randint(1, 11)
    pos = rng.sample(range(-10, 41), n)          # 位置互不相同, 允许负坐标
    d = rng.choice([0, 1, 2, 3, 5, 8, rng.randint(0, 30)])
    return f"{n} {d}\n" + " ".join(map(str, pos)) + "\n"

def run(exe, data):
    p = subprocess.run([exe], input=data, capture_output=True, text=True)
    return p.stdout.strip()

def main():
    iters = int(sys.argv[1]) if len(sys.argv) > 1 else 300
    rng = random.Random(20260101)
    for t in range(iters):
        data = gen(rng)
        a, b = run(SOL, data), run(BRU, data)
        if a != b:
            print("MISMATCH at", t)
            print(data)
            print("sol =", a, "brute =", b)
            return
    print(f"done {iters} tests, mismatches = 0")

main()
