#!/usr/bin/env python3
import random, subprocess, time, sys

STD = "/tmp/P1889"
BRU = "/tmp/P1889_brute"
SRC = "/tmp/luogu-ps/appendix/P1889/P1889.cpp"
BRUS = "/tmp/luogu-ps/appendix/P1889/P1889_brute.cpp"

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

def pack(pts):
    return str(len(pts)) + "\n" + "\n".join(f"{x} {y}" for x, y in pts) + "\n"

def main():
    if compile_all():
        return 1
    rng = random.Random(1889)
    tests = 0
    sample = "5\n1 2\n2 2\n1 3\n3 -2\n3 3\n"
    exp = "8\n"
    a = run(STD, sample)
    b = run(BRU, sample)
    if a != exp or b != exp:
        return fail(sample, a, b)
    tests += 1
    fixed = [
        [(0, 0)],
        [(5, -3)],
        [(0, 0), (2, 0)],
        [(0, 0), (0, 0)],
        [(1, 1), (2, 2), (3, 3)],
        [(-5, 4), (0, -2), (3, 3), (3, 3)],
        [(1, 0), (2, 0), (3, 0), (4, 0), (5, 0), (6, 0)],
    ]
    for pts in fixed:
        inp = pack(pts)
        a = run(STD, inp)
        b = run(BRU, inp)
        if a != b:
            return fail(inp, a, b)
        tests += 1
    for _ in range(40):
        n = rng.randint(1, 6)
        pts = [(rng.randint(-7, 7), rng.randint(-7, 7)) for _ in range(n)]
        inp = pack(pts)
        a = run(STD, inp)
        b = run(BRU, inp)
        if a != b:
            return fail(inp, a, b)
        tests += 1
    n = 10000
    pts = [(rng.randint(-10000, 10000), rng.randint(-10000, 10000)) for _ in range(n)]
    inp = pack(pts)
    t0 = time.perf_counter()
    a = run(STD, inp)
    dt = time.perf_counter() - t0
    if not a.strip().lstrip("-").isdigit():
        return fail("large", a, "")
    tests += 1
    print(f"max n=10000 time={dt:.4f}s ans={a.strip()}")
    print(f"{tests} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
