#!/usr/bin/env python3
import os, random, string, subprocess, sys, time

here = os.path.dirname(os.path.abspath(__file__))
SOL = "/tmp/P1808"
BRU = "/tmp/P1808_brute"
SRC = os.path.join(here, "P1808.cpp")
BSRC = os.path.join(here, "P1808_brute.cpp")

SAMPLE_IN = "3\nAABAC\nCBAAA\nAAABB\n"
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

def word(rng, L=None):
    if L is None:
        L = rng.randint(1, 12)
    return "".join(rng.choice(string.ascii_uppercase) for _ in range(L))

def gen(rng):
    n = rng.randint(1, 80)
    lines = [str(n)]
    base = [word(rng) for _ in range(max(1, n // 3))]
    for i in range(n):
        if rng.random() < 0.5:
            w = list(rng.choice(base))
            rng.shuffle(w)
            lines.append("".join(w))
        else:
            lines.append(word(rng))
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
        "1\nA\n",
        "2\nABC\nCBA\n",
        "3\nA\nB\nC\n",
        "4\nAAAA\nAAAA\nAAAA\nAAAA\n",
        "3\nABC\nABCD\nABC\n",
        "2\nZZ\nYY\n",
    ]
    for data in edges:
        a, b = run(SOL, data), run(BRU, data)
        tests += 1
        if a is None or b is None or a != b:
            print("EDGE FAIL", data, repr(a), repr(b))
            return 1
    rng = random.Random(1808008)
    t0 = time.time()
    for t in range(200):
        data = gen(rng)
        a, b = run(SOL, data), run(BRU, data)
        tests += 1
        if a is None or b is None or a != b:
            print("MISMATCH", t)
            print(data)
            print(repr(a), repr(b))
            return 1
    rnd_time = time.time() - t0
    rng2 = random.Random(4)
    n = 10000
    lines = [str(n)]
    for _ in range(n):
        lines.append(word(rng2, 100))
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
