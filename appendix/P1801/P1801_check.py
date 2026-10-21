#!/usr/bin/env python3
import os, random, subprocess, sys, time

here = os.path.dirname(os.path.abspath(__file__))
SOL = "/tmp/P1801"
BRU = "/tmp/P1801_brute"
SRC = os.path.join(here, "P1801.cpp")
BSRC = os.path.join(here, "P1801_brute.cpp")

SAMPLE_IN = "7 4\n3 1 -4 2 8 -1000 2\n1 2 6 6\n"
SAMPLE_OUT = "3\n3\n1\n2\n"

def compile_all():
    for src, out in ((SRC, SOL), (BSRC, BRU)):
        r = subprocess.run(["g++", "-O2", "-std=c++17", "-Wall", "-Wextra", "-o", out, src],
                           capture_output=True, text=True)
        if r.returncode != 0 or r.stderr.strip():
            sys.stderr.write(r.stderr)
            print("COMPILE FAIL", src)
            return 1
    return 0

def run(exe, data, timeout=30):
    p = subprocess.run([exe], input=data, capture_output=True, text=True, timeout=timeout)
    if p.returncode != 0:
        print("RUNTIME", exe, p.returncode, p.stderr[:400])
        print(data[:500])
        return None
    return p.stdout

def gen(rng):
    m = rng.randint(1, 500)
    n = rng.randint(1, m)
    a = [rng.randint(-2_000_000_000, 2_000_000_000) for _ in range(m)]
    u = []
    prev = 0
    for i in range(1, n + 1):
        lo = max(prev, i)
        hi = m - (n - i)
        u.append(rng.randint(lo, hi))
        prev = u[-1]
    lines = [f"{m} {n}", " ".join(map(str, a)), " ".join(map(str, u))]
    return "\n".join(lines) + "\n"

def main():
    if compile_all():
        return 1
    tests = 0
    got = run(SOL, SAMPLE_IN)
    if got != SAMPLE_OUT:
        print("SAMPLE FAIL")
        print("got", repr(got))
        print("exp", repr(SAMPLE_OUT))
        return 1
    bgot = run(BRU, SAMPLE_IN)
    if bgot != SAMPLE_OUT:
        print("SAMPLE BRUTE FAIL", repr(bgot))
        return 1
    tests += 1
    edges = []
    edges.append("1 1\n5\n1\n")
    edges.append("1 1\n-2000000000\n1\n")
    edges.append("3 3\n2 2 2\n1 2 3\n")
    edges.append("4 2\n9 1 8 3\n2 4\n")
    edges.append("5 4\n-5 -1 -3 0 2\n1 3 3 5\n")
    for data in edges:
        a, b = run(SOL, data), run(BRU, data)
        tests += 1
        if a is None or b is None or a != b:
            print("EDGE FAIL")
            print(data)
            print("sol", repr(a), "brute", repr(b))
            return 1
    rng = random.Random(1801001)
    t0 = time.time()
    rounds = 200
    for t in range(rounds):
        data = gen(rng)
        a, b = run(SOL, data), run(BRU, data)
        tests += 1
        if a is None or b is None or a != b:
            print("MISMATCH", t)
            print(data)
            print("sol", a)
            print("brute", b)
            return 1
    rnd_time = time.time() - t0
    m = n = 200000
    rng2 = random.Random(7)
    a = [str(rng2.randint(-2000000000, 2000000000)) for _ in range(m)]
    u = [str(i) for i in range(1, n + 1)]
    big = f"{m} {n}\n" + " ".join(a) + "\n" + " ".join(u) + "\n"
    t1 = time.time()
    out = run(SOL, big, timeout=60)
    tmax = time.time() - t1
    if out is None or out.count("\n") != n:
        print("LARGE FAIL", None if out is None else out.count("\n"))
        return 1
    print(f"TIME random={rnd_time:.3f}s large_m={m} time={tmax:.3f}s lines={n}")
    print(f"{tests} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
