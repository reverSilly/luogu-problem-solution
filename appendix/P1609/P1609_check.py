#!/usr/bin/env python3
# P1609 对拍：随机小数值 + 9/10 边界值，比较标程（构造法）与暴力（逐个枚举）
import random, subprocess, sys

SOL = "/tmp/P1609"
BRU = "/tmp/P1609_brute"

def run(exe, data):
    p = subprocess.run([exe], input=data, capture_output=True, text=True)
    return p.stdout.strip()

def main():
    iters = int(sys.argv[1]) if len(sys.argv) > 1 else 300
    rng = random.Random(31415)
    cases = []
    for d in range(1, 13):
        lo = 10 ** (d - 1)
        hi = 10 ** d - 1
        for _ in range(iters // 12 + 1):
            cases.append(rng.randint(lo, hi))
        cases.append(hi)                     # 全 9
        cases.append(lo)                     # 10...0
        cases.append(lo + 1)
        cases.append(max(lo, hi - 1))
    bad = 0
    for n in cases:
        data = f"{n}\n"
        a, b = run(SOL, data), run(BRU, data)
        if a != b:
            print("MISMATCH", n, "sol =", a, "brute =", b)
            bad += 1
            if bad > 5:
                return
    print(f"done {len(cases)} tests, mismatches = {bad}")

main()
