#!/usr/bin/env python3
import random, subprocess, time, sys

STD = "/tmp/P1897"
BRU = "/tmp/P1897_brute"
SRC = "/tmp/luogu-ps/appendix/P1897/P1897.cpp"
BRUS = "/tmp/luogu-ps/appendix/P1897/P1897_brute.cpp"

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
    print(inp[:1000])
    print("STD:")
    print(a)
    print("BRUTE:")
    print(b)
    return 1

def pack(a):
    return str(len(a)) + "\n" + " ".join(map(str, a)) + "\n"

def main():
    if compile_all():
        return 1
    rng = random.Random(1897)
    tests = 0
    sample = "4\n2 4 3 2\n"
    exp = "59\n"
    a = run(STD, sample)
    b = run(BRU, sample)
    if a != exp or b != exp:
        return fail(sample, a, b)
    tests += 1
    fixed = [
        [1],
        [5, 5, 5],
        [1, 2, 3, 4, 5],
        [7],
        [2, 2, 2, 9],
        [20000000],
    ]
    for arr in fixed:
        inp = pack(arr)
        oa = run(STD, inp)
        ob = run(BRU, inp)
        if oa != ob:
            return fail(inp, oa, ob)
        tests += 1
    for _ in range(40):
        n = rng.randint(1, 50)
        arr = [rng.randint(1, 30) for _ in range(n)]
        inp = pack(arr)
        oa = run(STD, inp)
        ob = run(BRU, inp)
        if oa != ob:
            return fail(inp, oa, ob)
        tests += 1
    for _ in range(15):
        n = rng.randint(1, 5000)
        arr = [rng.randint(1, 2 * 10**7) for _ in range(n)]
        inp = pack(arr)
        oa = run(STD, inp)
        ob = run(BRU, inp)
        if oa != ob:
            return fail(inp, oa, ob)
        tests += 1
    n = 99999
    arr = [rng.randint(1, 2 * 10**7) for _ in range(n)]
    inp = pack(arr)
    t0 = time.perf_counter()
    oa = run(STD, inp)
    dt = time.perf_counter() - t0
    ob = run(BRU, inp)
    if oa != ob:
        return fail("large", oa, ob)
    tests += 1
    print(f"max n=99999 time={dt:.4f}s ans={oa.strip()}")
    print(f"{tests} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
