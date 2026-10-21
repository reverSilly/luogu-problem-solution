#!/usr/bin/env python3
import random, subprocess, time, sys

STD = "/tmp/P1885"
BRU = "/tmp/P1885_brute"
SRC = "/tmp/luogu-ps/appendix/P1885/P1885.cpp"
BRUS = "/tmp/luogu-ps/appendix/P1885/P1885_brute.cpp"

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

def main():
    if compile_all():
        return 1
    rng = random.Random(1885)
    tests = 0
    samples = [("11\n", "m\n")]
    for inp, exp in samples:
        a = run(STD, inp)
        b = run(BRU, inp)
        if a != exp or b != exp:
            return fail(inp, a, b)
        tests += 1
    bounds = [1, 2, 3, 4, 5, 10, 11, 12, 25, 26, 27]
    nlen = 3
    k = 0
    while nlen < 200000:
        k += 1
        nlen = nlen * 2 + (k + 3)
        bounds.append(nlen - 1)
        bounds.append(nlen)
        bounds.append(nlen + 1)
    for n in bounds:
        if n < 1 or n > 300000:
            continue
        inp = f"{n}\n"
        a = run(STD, inp)
        b = run(BRU, inp)
        if a != b or a not in ("m\n", "o\n"):
            return fail(inp, a, b)
        tests += 1
    for _ in range(80):
        n = rng.randint(1, 5000)
        inp = f"{n}\n"
        a = run(STD, inp)
        b = run(BRU, inp)
        if a != b:
            return fail(inp, a, b)
        tests += 1
    for _ in range(40):
        n = rng.randint(5001, 250000)
        inp = f"{n}\n"
        a = run(STD, inp)
        b = run(BRU, inp)
        if a != b:
            return fail(inp, a, b)
        tests += 1
    big = "1000000000\n"
    t0 = time.perf_counter()
    a = run(STD, big)
    dt = time.perf_counter() - t0
    if a not in ("m\n", "o\n"):
        return fail(big, a, "")
    tests += 1
    print(f"max N=1000000000 time={dt:.4f}s ans={a.strip()}")
    print(f"{tests} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
