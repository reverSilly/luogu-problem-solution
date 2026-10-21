#!/usr/bin/env python3
import os, random, subprocess, sys, time

here = os.path.dirname(os.path.abspath(__file__))
SOL = "/tmp/P1807"
BRU = "/tmp/P1807_brute"
SRC = os.path.join(here, "P1807.cpp")
BSRC = os.path.join(here, "P1807_brute.cpp")

SAMPLE_IN = "2 1\n1 2 1\n"
SAMPLE_OUT = "1\n"

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
        print("RUNTIME", exe, p.returncode, p.stderr[:500])
        print(data[:500])
        return None
    return p.stdout

def gen(rng):
    n = rng.randint(1, 8)
    m = 0 if n == 1 else rng.randint(0, min(16, n * (n - 1) // 2))
    cand = [(u, v) for u in range(1, n + 1) for v in range(u + 1, n + 1)]
    rng.shuffle(cand)
    edges = cand[:m]
    # sometimes duplicate an edge
    if edges and rng.random() < 0.4:
        edges.append(edges[rng.randrange(len(edges))])
        m = len(edges)
    lines = [f"{n} {m}"]
    for u, v in edges:
        w = rng.randint(-20, 20)
        lines.append(f"{u} {v} {w}")
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
        "1 0\n",
        "2 0\n",
        "3 2\n1 2 -5\n2 3 -5\n",
        "3 3\n1 2 -5\n2 3 -5\n1 3 -3\n",
        "3 1\n1 2 5\n",
        "4 3\n1 2 10\n2 4 -100\n1 4 1\n",
        "4 2\n1 2 5\n3 4 100\n",
        "3 2\n1 3 4\n1 3 -2\n",
        "5 4\n1 2 -1\n2 3 -1\n3 4 -1\n4 5 10\n",
    ]
    for data in edges:
        a, b = run(SOL, data), run(BRU, data)
        tests += 1
        if a is None or b is None or a != b:
            print("EDGE FAIL")
            print(data, repr(a), repr(b))
            return 1
    rng = random.Random(1807007)
    t0 = time.time()
    for t in range(400):
        data = gen(rng)
        a, b = run(SOL, data), run(BRU, data)
        tests += 1
        if a is None or b is None or a != b:
            print("MISMATCH", t)
            print(data)
            print(repr(a), repr(b))
            return 1
    rnd_time = time.time() - t0
    rng2 = random.Random(9)
    n, m = 1500, 50000
    lines = [f"{n} {m}"]
    for _ in range(m):
        u = rng2.randint(1, n - 1)
        v = rng2.randint(u + 1, n)
        w = rng2.randint(-100000, 100000)
        lines.append(f"{u} {v} {w}")
    big = "\n".join(lines) + "\n"
    t1 = time.time()
    out = run(SOL, big, timeout=30)
    tmax = time.time() - t1
    if out is None or not (out.strip().lstrip("-").isdigit()):
        print("LARGE FAIL", out)
        return 1
    print(f"TIME random={rnd_time:.3f}s large_n={n} time={tmax:.3f}s ans={out.strip()}")
    print(f"{tests} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
