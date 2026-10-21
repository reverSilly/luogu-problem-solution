#!/usr/bin/env python3
# P1611 对拍：随机同位数区间 [A,B]（B 不大），比较标程（枚举旋转）与暴力（枚举数对）
import random, subprocess, sys

SOL = "/tmp/P1611"
BRU = "/tmp/P1611_brute"

def run(exe, data):
    p = subprocess.run([exe], input=data, capture_output=True, text=True)
    return p.stdout.strip()

def main():
    iters = int(sys.argv[1]) if len(sys.argv) > 1 else 200
    rng = random.Random(987654)
    cases = []
    for d in range(1, 5):
        lo = 10 ** (d - 1)
        hi = 10 ** d - 1
        for _ in range(iters // 4):
            a = rng.randint(lo, hi)
            b = rng.randint(a, min(hi, a + 3000))
            cases.append((a, b))
        cases.append((lo, min(hi, lo + 1500)))
        cases.append((hi - 1500, hi))
        if d == 3:
            cases.append((120, 210))          # 周期串密集区
    bad = 0
    for a, b in cases:
        data = f"{a} {b}\n"
        x, y = run(SOL, data), run(BRU, data)
        if x != y:
            print("MISMATCH", a, b, "sol =", x, "brute =", y)
            bad += 1
            if bad > 5:
                return
    print(f"done {len(cases)} tests, mismatches = {bad}")

main()
