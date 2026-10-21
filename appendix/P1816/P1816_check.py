#!/usr/bin/env python3
import os, random, subprocess, sys, time

here = os.path.dirname(os.path.abspath(__file__))
SOL = "/tmp/P1816"
BRU = "/tmp/P1816_brute"
SRC = os.path.join(here, "P1816.cpp")
BSRC = os.path.join(here, "P1816_brute.cpp")

SAMPLE_IN = "10 3\n1 2 3 4 5 6 7 8 9 10\n2 7\n3 9\n1 10\n"
SAMPLE_OUT = "2 3 1\n"

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
        print(data[:400])
        return None
    return p.stdout

def gen(rng):
    m = rng.randint(1, 400)
    q = rng.randint(1, 400)
    a = [rng.randint(0, 100000) for _ in range(m)]
    lines = [f"{m} {q}", " ".join(map(str, a))]
    for _ in range(q):
        l = rng.randint(1, m)
        r = rng.randint(1, m)
        if rng.random() < 0.5:
            l, r = r, l
        lines.append(f"{l} {r}")
    return "\n".join(lines) + "\n"

def main():
    if compile_all():
        return 1
    tests = 0
    if run(SOL, SAMPLE_IN) != SAMPLE_OUT or run(BRU, SAMPLE_IN) != SAMPLE_OUT:
        print("SAMPLE FAIL", repr(run(SOL, SAMPLE_IN)), repr(run(BRU, SAMPLE_IN)))
        return 1
    tests += 1
    edges = [
        "1 1\n0\n1 1\n",
        "1 1\n7\n1 1\n",
        "5 3\n5 4 3 2 1\n5 1\n3 3\n1 5\n",
        "4 2\n0 0 0 0\n2 4\n4 2\n",
        "6 1\n9 8 7 1 6 5\n6 2\n",
    ]
    for data in edges:
        a, b = run(SOL, data), run(BRU, data)
        tests += 1
        if a is None or b is None or a != b:
            print("EDGE FAIL", data, repr(a), repr(b))
            return 1
    rng = random.Random(1816016)
    t0 = time.time()
    for t in range(200):
        data = gen(rng)
        a, b = run(SOL, data), run(BRU, data)
        tests += 1
        if a is None or b is None or a != b:
            print("MISMATCH", t)
            print(data[:500])
            print(repr(a), repr(b))
            return 1
    rnd_time = time.time() - t0
    rng2 = random.Random(5)
    m = n = 100000
    a = [str(rng2.randint(0, 100000)) for _ in range(m)]
    lines = [f"{m} {n}", " ".join(a)]
    for _ in range(n):
        l = rng2.randint(1, m)
        r = rng2.randint(1, m)
        if l > r:
            l, r = r, l
        lines.append(f"{l} {r}")
    big = "\n".join(lines) + "\n"
    t1 = time.time()
    out = run(SOL, big, timeout=30)
    tmax = time.time() - t1
    if out is None or len(out.split()) != n:
        print("LARGE FAIL", None if out is None else len(out.split()))
        return 1
    # spot-check 20 queries against brute on a prefix? full brute too slow.
    # recompute a few answers in python
    arr = list(map(int, a))
    qs = lines[2:]
    ans = out.split()
    for idx in list(range(0, n, n // 20))[:20]:
        l, r = map(int, qs[idx].split())
        if l > r:
            l, r = r, l
        expect = min(arr[l - 1:r])
        if int(ans[idx]) != expect:
            print("SPOT FAIL", idx, l, r, ans[idx], expect)
            return 1
    print(f"TIME random={rnd_time:.3f}s large_m={m} time={tmax:.3f}s")
    print(f"{tests} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
