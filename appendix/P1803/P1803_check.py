#!/usr/bin/env python3
import os, random, subprocess, sys, time

here = os.path.dirname(os.path.abspath(__file__))
SOL = "/tmp/P1803"
BRU = "/tmp/P1803_brute"
SRC = os.path.join(here, "P1803.cpp")
BSRC = os.path.join(here, "P1803_brute.cpp")

SAMPLE_IN = "3\n0 2\n2 4\n1 3\n"
SAMPLE_OUT = "2\n"

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
    n = rng.randint(1, 16)
    lines = [str(n)]
    for _ in range(n):
        l = rng.randint(0, 40)
        r = rng.randint(l + 1, 50)
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
        "1\n0 1\n",
        "1\n5 6\n",
        "2\n0 2\n2 4\n",
        "2\n0 5\n1 2\n",
        "3\n1 2\n2 3\n3 4\n",
        "4\n0 10\n1 2\n2 3\n3 4\n",
        "3\n0 1\n0 1\n0 1\n",
        "5\n0 1000000\n1 2\n2 3\n3 4\n4 5\n",
    ]
    for data in edges:
        a, b = run(SOL, data), run(BRU, data)
        tests += 1
        if a is None or b is None or a != b:
            print("EDGE FAIL", data, repr(a), repr(b))
            return 1
    rng = random.Random(1803003)
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
    rng2 = random.Random(11)
    n = 1000000
    lines = [str(n)]
    for _ in range(n):
        l = rng2.randint(0, 999999)
        r = rng2.randint(l + 1, 1000000)
        lines.append(f"{l} {r}")
    big = "\n".join(lines) + "\n"
    t1 = time.time()
    out = run(SOL, big, timeout=60)
    tmax = time.time() - t1
    if out is None or not out.strip().isdigit():
        print("LARGE FAIL", out)
        return 1
    print(f"TIME random={rnd_time:.3f}s large_n={n} time={tmax:.3f}s ans={out.strip()}")
    print(f"{tests} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
