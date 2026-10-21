#!/usr/bin/env python3
import random, subprocess, time, sys

STD = "/tmp/P1886"
BRU = "/tmp/P1886_brute"
SRC = "/tmp/luogu-ps/appendix/P1886/P1886.cpp"
BRUS = "/tmp/luogu-ps/appendix/P1886/P1886_brute.cpp"

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

def make(n, k, arr):
    return f"{n} {k}\n" + " ".join(map(str, arr)) + "\n"

def main():
    if compile_all():
        return 1
    rng = random.Random(1886)
    tests = 0
    sample = "8 3\n1 3 -1 -3 5 3 6 7\n"
    exp = "-1 -3 -3 -3 3 3\n3 3 5 5 6 7\n"
    a = run(STD, sample)
    b = run(BRU, sample)
    if a != exp or b != exp:
        return fail(sample, a, b)
    tests += 1
    specials = []
    specials.append(make(1, 1, [0]))
    specials.append(make(1, 1, [-2147483648]))
    specials.append(make(1, 1, [2147483647]))
    specials.append(make(5, 1, [3, 1, 4, 1, 5]))
    specials.append(make(5, 5, [3, 1, 4, 1, 5]))
    specials.append(make(6, 2, [1, 1, 1, 1, 1, 1]))
    specials.append(make(6, 3, [5, 4, 3, 2, 1, 0]))
    specials.append(make(6, 3, [0, 1, 2, 3, 4, 5]))
    specials.append(make(4, 2, [-2147483648, 2147483647, -2147483648, 7]))
    for inp in specials:
        a = run(STD, inp)
        b = run(BRU, inp)
        if a != b:
            return fail(inp, a, b)
        tests += 1
    for _ in range(60):
        n = rng.randint(1, 400)
        k = rng.randint(1, n)
        arr = [rng.randint(-1000, 1000) for _ in range(n)]
        inp = make(n, k, arr)
        a = run(STD, inp)
        b = run(BRU, inp)
        if a != b:
            return fail(inp, a, b)
        tests += 1
    for _ in range(20):
        n = rng.randint(1, 2000)
        k = rng.randint(1, min(n, 30))
        arr = [rng.randint(-10**9, 10**9) for _ in range(n)]
        if rng.randrange(5) == 0:
            arr[rng.randrange(n)] = -2147483648
        inp = make(n, k, arr)
        a = run(STD, inp)
        b = run(BRU, inp)
        if a != b:
            return fail(inp, a, b)
        tests += 1
    n = 1000000
    arr = [rng.randint(-10**9, 10**9) for _ in range(n)]
    arr[0] = -2147483648
    arr[-1] = 2147483647
    cases = [(1, True), (3, True), (n, True), (1000, False)]
    dt_max = 0.0
    note = ""
    for k, use_brute in cases:
        inp = make(n, k, arr)
        t0 = time.perf_counter()
        a = run(STD, inp)
        dt = time.perf_counter() - t0
        if dt > dt_max:
            dt_max = dt
            note = f"k={k}"
        if use_brute:
            b = run(BRU, inp)
            if a != b:
                return fail(inp[:200], a[:500], b[:500])
        else:
            lines = a.splitlines()
            if len(lines) != 2:
                return fail(inp[:100], a[:200], "bad lines")
            if len(lines[0].split()) != n - k + 1 or len(lines[1].split()) != n - k + 1:
                return fail(inp[:100], a[:200], "bad width")
        tests += 1
    print(f"max n=1000000 slowest {note} time={dt_max:.4f}s")
    print(f"{tests} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
