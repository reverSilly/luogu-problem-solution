#!/usr/bin/env python3
# P1614 对拍：随机 n,m（含 m=0、m=n），比较标程（滑动窗口）与暴力（逐个起点累加）
import random, subprocess, sys

SOL = "/tmp/P1614"
BRU = "/tmp/P1614_brute"

def run(exe, data):
    p = subprocess.run([exe], input=data, capture_output=True, text=True)
    return p.stdout.strip()

def main():
    iters = int(sys.argv[1]) if len(sys.argv) > 1 else 300
    rng = random.Random(2024)
    for t in range(iters):
        n = rng.randint(1, 40)
        m = rng.choice([0, 1, n, rng.randint(0, n)])
        a = [rng.randint(1, 100) for _ in range(n)]
        data = f"{n} {m}\n" + "\n".join(map(str, a)) + "\n"
        x, y = run(SOL, data), run(BRU, data)
        if x != y:
            print("MISMATCH", data, "sol =", x, "brute =", y)
            return
    print(f"done {iters} tests, mismatches = 0")

main()
