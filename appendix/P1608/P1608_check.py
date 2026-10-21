#!/usr/bin/env python3
# P1608 对拍：随机小图（含重边、自环输入、不可达情形），比较标程与暴力枚举点序列
import random, subprocess, sys

SOL = "/tmp/P1608"
BRU = "/tmp/P1608_brute"

def gen(rng):
    n = rng.randint(1, 6)
    pairs = []
    if n >= 2:
        for a in range(1, n + 1):
            for b in range(1, n + 1):
                if a != b:
                    pairs.append((a, b))
    m = rng.randint(0, 9) if pairs else 0
    lines = [f"{n} {m}"]
    for _ in range(m):
        u, v = rng.choice(pairs)
        c = rng.randint(1, 5)
        lines.append(f"{u} {v} {c}")
    return "\n".join(lines) + "\n"

def run(exe, data):
    p = subprocess.run([exe], input=data, capture_output=True, text=True)
    return p.stdout.strip()

def main():
    iters = int(sys.argv[1]) if len(sys.argv) > 1 else 300
    rng = random.Random(778899)
    for t in range(iters):
        data = gen(rng)
        a, b = run(SOL, data), run(BRU, data)
        if a != b:
            print("MISMATCH at test", t)
            print(data)
            print("sol =", repr(a), "brute =", repr(b))
            return
    print(f"done {iters} tests, mismatches = 0")

main()
