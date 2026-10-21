#!/usr/bin/env python3
import math, random, subprocess, time, sys

STD = "/tmp/P1888"
BRU = "/tmp/P1888_brute"
SRC = "/tmp/luogu-ps/appendix/P1888/P1888.cpp"
BRUS = "/tmp/luogu-ps/appendix/P1888/P1888_brute.cpp"

def compile_all():
    for src, dst in ((SRC, STD), (BRUS, BRU)):
        r = subprocess.run(
            ["g++", "-O2", "-std=c++17", "-Wall", "-Wextra", "-Werror", "-o", dst, src],
            capture_output=True, text=True,
        )
        if r.returncode != 0:
            sys.stderr.write(r.stderr)
            return 1
    return 0

def run(bin, inp):
    p = subprocess.run([bin], input=inp, text=True, capture_output=True)
    if p.returncode != 0:
        raise RuntimeError(p.stderr)
    return p.stdout

def fail(inp, a, b):
    print("FAIL")
    print("INPUT:")
    print(inp)
    print("STD:")
    print(a)
    print("BRUTE:")
    print(b)
    return 1

def triples(rng, limit):
    out = []
    for m in range(2, 40):
        for n in range(1, m):
            if (m - n) % 2 == 0 or math.gcd(m, n) != 1:
                continue
            a = m * m - n * n
            b = 2 * m * n
            c = m * m + n * n
            if c > limit:
                continue
            kmax = limit // c
            ks = {1}
            if kmax >= 2:
                ks.add(2)
            if kmax >= 3:
                ks.add(rng.randint(2, kmax))
            for k in ks:
                trip = [a * k, b * k, c * k]
                rng.shuffle(trip)
                out.append(trip)
    return out

def main():
    if compile_all():
        return 1
    rng = random.Random(1888)
    tests = 0
    sample = "3 5 4\n"
    exp = "3/5\n"
    a = run(STD, sample)
    b = run(BRU, sample)
    if a != exp or b != exp:
        return fail(sample, a, b)
    tests += 1
    manual = [
        "6 8 10\n",
        "5 12 13\n",
        "8 15 17\n",
        "7 24 25\n",
        "20 21 29\n",
        "9 40 41\n",
        "12 35 37\n",
        "9 12 15\n",
        "28 45 53\n",
    ]
    for inp in manual:
        a = run(STD, inp)
        b = run(BRU, inp)
        if a != b or "/" not in a.strip():
            return fail(inp, a, b)
        tests += 1
    for trip in triples(rng, 10**9):
        xs = trip[:]
        rng.shuffle(xs)
        inp = f"{xs[0]} {xs[1]} {xs[2]}\n"
        a = run(STD, inp)
        b = run(BRU, inp)
        if a != b:
            return fail(inp, a, b)
        tests += 1
    inp = "799960001 39999 799960000\n"
    t0 = time.perf_counter()
    a = run(STD, inp)
    dt = time.perf_counter() - t0
    b = run(BRU, inp)
    if a != b:
        return fail(inp, a, b)
    tests += 1
    print(f"big triple time={dt:.4f}s ans={a.strip()}")
    print(f"{tests} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
