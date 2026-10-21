#!/usr/bin/env python3
import os, random, subprocess, sys, time

here = os.path.dirname(os.path.abspath(__file__))
SOL = "/tmp/P1802"
BRU = "/tmp/P1802_brute"
SRC = os.path.join(here, "P1802.cpp")
BSRC = os.path.join(here, "P1802_brute.cpp")

SAMPLE_IN = "6 8\n21 52 1\n21 70 5\n21 48 2\n14 38 3\n14 36 1\n14 36 2\n"
SAMPLE_OUT = "1060\n"

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
        print(data[:800])
        return None
    return p.stdout

def gen(rng):
    n = rng.randint(0, 16)
    x = rng.randint(0, 30)
    lines = [f"{n} {x}"]
    for _ in range(n):
        lose = rng.randint(1, 50)
        win = rng.randint(lose, 80)
        use = rng.randint(0, 12)
        lines.append(f"{lose} {win} {use}")
    return "\n".join(lines) + "\n"

def main():
    if compile_all():
        return 1
    tests = 0
    got = run(SOL, SAMPLE_IN)
    if got != SAMPLE_OUT or run(BRU, SAMPLE_IN) != SAMPLE_OUT:
        print("SAMPLE FAIL", repr(got), repr(run(BRU, SAMPLE_IN)))
        return 1
    tests += 1
    edges = [
        "0 0\n",
        "0 5\n",
        "1 0\n3 9 0\n",
        "1 0\n3 9 1\n",
        "2 0\n1 1 0\n4 8 0\n",
        "3 5\n1 100 0\n1 100 0\n1 100 6\n",
        "4 3\n5 5 1\n5 6 2\n5 9 3\n5 20 4\n",
    ]
    for data in edges:
        a, b = run(SOL, data), run(BRU, data)
        tests += 1
        if a is None or b is None or a != b:
            print("EDGE FAIL")
            print(data, repr(a), repr(b))
            return 1
    rng = random.Random(1802002)
    t0 = time.time()
    for t in range(300):
        data = gen(rng)
        a, b = run(SOL, data), run(BRU, data)
        tests += 1
        if a is None or b is None or a != b:
            print("MISMATCH", t)
            print(data)
            print(repr(a), repr(b))
            return 1
    rnd_time = time.time() - t0
    rng2 = random.Random(3)
    n = x = 1000
    lines = [f"{n} {x}"]
    for _ in range(n):
        lose = rng2.randint(1, 1000000)
        win = rng2.randint(lose, 1000000)
        use = rng2.randint(0, 1000)
        lines.append(f"{lose} {win} {use}")
    big = "\n".join(lines) + "\n"
    t1 = time.time()
    out = run(SOL, big, timeout=30)
    tmax = time.time() - t1
    if out is None or not out.strip().isdigit():
        print("LARGE FAIL", out)
        return 1
    print(f"TIME random={rnd_time:.3f}s large_n={n} time={tmax:.3f}s ans={out.strip()}")
    print(f"{tests} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
