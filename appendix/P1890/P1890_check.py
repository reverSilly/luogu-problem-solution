#!/usr/bin/env python3
import math, random, subprocess, time, sys

STD = "/tmp/P1890"
BRU = "/tmp/P1890_brute"
SRC = "/tmp/luogu-ps/appendix/P1890/P1890.cpp"
BRUS = "/tmp/luogu-ps/appendix/P1890/P1890_brute.cpp"

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
    print(a[:2000])
    print("BRUTE:")
    print(b[:2000])
    return 1

def pack(a, qs):
    lines = [f"{len(a)} {len(qs)}", " ".join(map(str, a))]
    for l, r in qs:
        lines.append(f"{l} {r}")
    return "\n".join(lines) + "\n"

def main():
    if compile_all():
        return 1
    rng = random.Random(1890)
    tests = 0
    sample = "5 3\n4 12 3 6 7\n1 3\n2 3\n5 5\n"
    exp = "1\n3\n7\n"
    a = run(STD, sample)
    b = run(BRU, sample)
    if a != exp or b != exp:
        return fail(sample, a, b)
    tests += 1
    fixed = [
        ([7], [(1, 1)]),
        ([1, 1, 1], [(1, 3), (2, 2)]),
        ([8, 12, 18, 24], [(1, 4), (1, 2), (3, 4), (2, 2)]),
        ([10**9, 10**9 - 1, 2], [(1, 1), (1, 3), (2, 3)]),
    ]
    for arr, qs in fixed:
        inp = pack(arr, qs)
        oa = run(STD, inp)
        ob = run(BRU, inp)
        if oa != ob:
            return fail(inp, oa, ob)
        tests += 1
    for _ in range(40):
        n = rng.randint(1, 80)
        m = rng.randint(1, 40)
        arr = [rng.randint(1, 10**6) for _ in range(n)]
        qs = []
        for _j in range(m):
            l = rng.randint(1, n)
            r = rng.randint(l, n)
            qs.append((l, r))
        inp = pack(arr, qs)
        oa = run(STD, inp)
        ob = run(BRU, inp)
        if oa != ob:
            return fail(inp, oa, ob)
        tests += 1
    for _ in range(10):
        n = rng.randint(1, 1000)
        m = rng.randint(1, 200)
        arr = [rng.randint(1, 10**9) for _ in range(n)]
        qs = []
        for _j in range(m):
            l = rng.randint(1, n)
            r = rng.randint(l, n)
            qs.append((l, r))
        inp = pack(arr, qs)
        oa = run(STD, inp)
        ob = run(BRU, inp)
        if oa != ob:
            return fail(inp, oa, ob)
        tests += 1
    n = 1000
    m = 1000000
    arr = [rng.randint(1, 10**9) for _ in range(n)]
    # sprinkle small factors so gcd is nontrivial
    for i in range(n):
        if i % 3 == 0:
            arr[i] = rng.randint(1, 20) * 6
    qs_idx = []
    lines = [f"{n} {m}", " ".join(map(str, arr))]
    check_qs = []
    for i in range(m):
        l = rng.randint(1, n)
        r = rng.randint(l, n)
        lines.append(f"{l} {r}")
        if i < 30 or i % 50000 == 0:
            check_qs.append((i, l, r))
    inp = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    oa = run(STD, inp)
    dt = time.perf_counter() - t0
    outs = oa.splitlines()
    if len(outs) != m:
        return fail("large", f"lines {len(outs)}", "")
    for i, l, r in check_qs:
        g = arr[l - 1]
        for x in arr[l - 1:r]:
            g = math.gcd(g, x)
        if int(outs[i]) != g:
            return fail(f"q {l} {r}", outs[i], str(g))
    tests += 1
    print(f"max n=1000 m=1000000 time={dt:.4f}s")
    print(f"{tests} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
