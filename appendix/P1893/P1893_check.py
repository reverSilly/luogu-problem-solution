#!/usr/bin/env python3
import random, subprocess, time, sys

STD = "/tmp/P1893"
BRU = "/tmp/P1893_brute"
SRC = "/tmp/luogu-ps/appendix/P1893/P1893.cpp"
BRUS = "/tmp/luogu-ps/appendix/P1893/P1893_brute.cpp"

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
    print(inp[:2000])
    print("STD:")
    print(a)
    print("BRUTE:")
    print(b)
    return 1

def pack(h):
    return str(len(h)) + "\n" + "\n".join(map(str, h)) + "\n"

def main():
    if compile_all():
        return 1
    rng = random.Random(1893)
    tests = 0
    sample = "7\n3\n2\n3\n5\n4\n1\n6\n"
    exp = "5\n"
    a = run(STD, sample)
    b = run(BRU, sample)
    if a != exp or b != exp:
        return fail(sample, a, b)
    tests += 1
    fixed = [
        [1],
        [5, 5, 5, 5],
        [1, 2, 3, 4, 5],
        [5, 4, 3, 2, 1],
        [3, 1, 1, 2, 3, 4],
        [1, 3, 2, 2, 4],
        [1, 2, 2, 3, 1, 1, 1, 2, 3, 4],
        [2, 3, 5, 4, 1],
        [9, 1, 2, 3, 2, 1, 8],
        [1, 2, 1, 2, 1, 2],
    ]
    for h in fixed:
        inp = pack(h)
        oa = run(STD, inp)
        ob = run(BRU, inp)
        if oa != ob:
            return fail(inp, oa, ob)
        tests += 1
    for _ in range(30):
        n = rng.randint(1, 80)
        h = [rng.randint(1, 20) for _ in range(n)]
        inp = pack(h)
        oa = run(STD, inp)
        ob = run(BRU, inp)
        if oa != ob:
            return fail(inp, oa, ob)
        tests += 1
    for _ in range(8):
        n = rng.randint(80, 220)
        h = [rng.randint(1, 10**9) for _ in range(n)]
        inp = pack(h)
        oa = run(STD, inp)
        ob = run(BRU, inp)
        if oa != ob:
            return fail(inp, oa, ob)
        tests += 1
    n = 10000
    h = [rng.randint(1, 10**9) for _ in range(n)]
    inp = pack(h)
    t0 = time.perf_counter()
    oa = run(STD, inp)
    dt = time.perf_counter() - t0
    if not oa.strip().isdigit() or not (1 <= int(oa) <= n):
        return fail("large", oa, "")
    tests += 1
    print(f"max n=10000 time={dt:.4f}s ans={oa.strip()}")
    print(f"{tests} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
