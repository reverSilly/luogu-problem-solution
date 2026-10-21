#!/usr/bin/env python3
import os, subprocess, sys, time

here = os.path.dirname(os.path.abspath(__file__))
SOL = "/tmp/P1806"
BRU = "/tmp/P1806_brute"
SRC = os.path.join(here, "P1806.cpp")
BSRC = os.path.join(here, "P1806_brute.cpp")

SAMPLE_IN = "212\n"
SAMPLE_OUT = "995645335\n"

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
        return None
    return p.stdout

def main():
    if compile_all():
        return 1
    tests = 0
    if run(SOL, SAMPLE_IN) != SAMPLE_OUT or run(BRU, SAMPLE_IN) != SAMPLE_OUT:
        print("SAMPLE FAIL", repr(run(SOL, SAMPLE_IN)), repr(run(BRU, SAMPLE_IN)))
        return 1
    tests += 1
    t0 = time.time()
    for n in range(5, 501):
        data = f"{n}\n"
        a, b = run(SOL, data), run(BRU, data)
        tests += 1
        if a is None or b is None or a != b:
            print("MISMATCH", n, repr(a), repr(b))
            return 1
    elapsed = time.time() - t0
    t1 = time.time()
    out = run(SOL, "500\n")
    tmax = time.time() - t1
    print(f"TIME all_n={elapsed:.3f}s n=500 time={tmax:.3f}s ans={out.strip()}")
    print(f"{tests} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
