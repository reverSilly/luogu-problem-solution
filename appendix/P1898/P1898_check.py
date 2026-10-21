#!/usr/bin/env python3
import random, subprocess, time, sys

STD = "/tmp/P1898"
BRU = "/tmp/P1898_brute"
SRC = "/tmp/luogu-ps/appendix/P1898/P1898.cpp"
BRUS = "/tmp/luogu-ps/appendix/P1898/P1898_brute.cpp"

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
    rng = random.Random(1898)
    tests = 0
    samples = [
        ("DLLSS\n478\n", "100\n"),
        ("JYFTYR\n81\n", "1\n"),
    ]
    for inp, exp in samples:
        a = run(STD, inp)
        b = run(BRU, inp)
        if a != exp or b != exp:
            return fail(inp, a, b)
        tests += 1
    letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    fixed = [
        ("AAAA\n1\n"),
        ("ZZZZ\n10000\n"),
        ("ABCD\n81\n"),
        ("MMMMMMMMMM\n2\n"),
        ("STUV\n9999\n"),
        ("ABCDE\n10\n"),
    ]
    for inp in fixed:
        a = run(STD, inp)
        b = run(BRU, inp)
        if a != b or not a.strip().isdigit():
            return fail(inp, a, b)
        tests += 1
    for _ in range(80):
        L = rng.randint(4, 10)
        s = "".join(rng.choice(letters) for _ in range(L))
        st = rng.randint(1, 10000)
        inp = f"{s}\n{st}\n"
        a = run(STD, inp)
        b = run(BRU, inp)
        if a != b:
            return fail(inp, a, b)
        tests += 1
    t0 = time.perf_counter()
    a = run(STD, "ZZZZZZZZZZ\n10000\n")
    dt = time.perf_counter() - t0
    b = run(BRU, "ZZZZZZZZZZ\n10000\n")
    if a != b:
        return fail("ZZZZZZZZZZ\n10000\n", a, b)
    tests += 1
    print(f"max L=10 ST=10000 time={dt:.4f}s ans={a.strip()}")
    print(f"{tests} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
