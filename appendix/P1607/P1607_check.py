#!/usr/bin/env python3
# P1607 对拍：随机小组数据，比较标程（贪心+线段树）与暴力（最小费用最大流）
import random, subprocess, sys, os

SOL = "/tmp/P1607"
BRU = "/tmp/P1607_brute"
TMP = "/tmp/P1607_in.txt"

def gen(rng):
    n = rng.randint(2, 8)
    c = rng.randint(1, 4)
    k = rng.randint(1, 8)
    lines = [f"{k} {n} {c}"]
    for _ in range(k):
        s = rng.randint(1, n - 1)
        e = rng.randint(s + 1, n)
        m = rng.randint(1, 3)
        lines.append(f"{s} {e} {m}")
    return "\n".join(lines) + "\n"

def run(exe, data):
    p = subprocess.run([exe], input=data, capture_output=True, text=True)
    return p.stdout.strip(), p.stderr

def main():
    iters = int(sys.argv[1]) if len(sys.argv) > 1 else 200
    rng = random.Random(20241021)
    bad = 0
    for t in range(iters):
        data = gen(rng)
        a, ea = run(SOL, data)
        b, eb = run(BRU, data)
        if ea.strip():
            print("stderr of sol:", ea); bad += 1; break
        if a != b:
            print("MISMATCH at test", t)
            print(data)
            print("sol =", a, "brute =", b)
            bad += 1
            break
    print(f"done {iters} tests, mismatches = {bad}")

main()
