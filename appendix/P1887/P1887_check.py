#!/usr/bin/env python3
import random, subprocess, time, sys

STD = "/tmp/P1887"
BRU = "/tmp/P1887_brute"
SRC = "/tmp/luogu-ps/appendix/P1887/P1887.cpp"
BRUS = "/tmp/luogu-ps/appendix/P1887/P1887_brute.cpp"

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
    print(a[:500])
    print("OTHER:")
    print(b[:500])
    return 1

def prop(inp, out, n, m):
    parts = out.strip().split()
    if len(parts) != m:
        return False
    if out != " ".join(parts) + "\n":
        return False
    nums = list(map(int, parts))
    if sum(nums) != n:
        return False
    if any(x < 1 for x in nums):
        return False
    if nums != sorted(nums):
        return False
    if nums[-1] - nums[0] > 1:
        return False
    return True

def main():
    if compile_all():
        return 1
    rng = random.Random(1887)
    tests = 0
    sample = "10 3\n"
    exp = "3 3 4\n"
    a = run(STD, sample)
    b = run(BRU, sample)
    if a != exp or b != exp:
        return fail(sample, a, b)
    tests += 1
    fixed = [(1, 1), (5, 1), (5, 5), (6, 2), (6, 3), (8, 3), (9, 4), (20, 7), (40, 13), (60, 60), (60, 1), (7, 3)]
    for n, m in fixed:
        inp = f"{n} {m}\n"
        a = run(STD, inp)
        b = run(BRU, inp)
        if a != b or not prop(inp, a, n, m):
            return fail(inp, a, b)
        tests += 1
    for _ in range(80):
        m = rng.randint(1, 40)
        n = rng.randint(m, 60)
        inp = f"{n} {m}\n"
        a = run(STD, inp)
        b = run(BRU, inp)
        if a != b or not prop(inp, a, n, m):
            return fail(inp, a, b)
        tests += 1
    larges = [(10**9, 1), (10**9, 10**6), (10**9, 3), (10**6, 10**6), (999999937, 123456)]
    dt_max = 0.0
    for n, m in larges:
        inp = f"{n} {m}\n"
        t0 = time.perf_counter()
        a = run(STD, inp)
        dt = time.perf_counter() - t0
        if dt > dt_max:
            dt_max = dt
        if not prop(inp, a, n, m):
            return fail(inp, a, "prop")
        tests += 1
    print(f"max N=1e9 M=1e6 time={dt_max:.4f}s")
    print(f"{tests} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
