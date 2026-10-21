#!/usr/bin/env python3
# P1612 对拍：随机树（含链、星形、随机父指针），权值/参数随机，比较标程（倍增）与暴力（逐级上跳）
import random, subprocess, sys

SOL = "/tmp/P1612"
BRU = "/tmp/P1612_brute"

def gen(rng):
    n = rng.randint(1, 40)
    mode = rng.choice(["chain", "star", "rand", "rand"])
    par = []
    for i in range(2, n + 1):
        if mode == "chain":
            par.append(i - 1)
        elif mode == "star":
            par.append(1)
        else:
            par.append(rng.randint(1, i - 1))
    w = [rng.randint(1, 20) for _ in range(n)]
    c = [rng.randint(w[i], w[i] + rng.choice([0, 3, 10, 50, 200])) for i in range(n)]
    return f"{n}\n" + " ".join(map(str, par)) + "\n" + " ".join(map(str, w)) + "\n" + " ".join(map(str, c)) + "\n"

def run(exe, data):
    p = subprocess.run([exe], input=data, capture_output=True, text=True)
    return p.stdout.strip()

def main():
    iters = int(sys.argv[1]) if len(sys.argv) > 1 else 300
    rng = random.Random(13579)
    for t in range(iters):
        data = gen(rng)
        a, b = run(SOL, data), run(BRU, data)
        if a != b:
            print("MISMATCH at", t)
            print(data)
            print("sol =", a)
            print("brute =", b)
            return
    print(f"done {iters} tests, mismatches = 0")

main()
