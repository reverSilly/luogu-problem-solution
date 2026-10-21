#!/usr/bin/env python3
import os
import random
import subprocess
import sys
import tempfile
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1655.cpp")
SRC_BRUTE = os.path.join(BASE, "P1655_brute.cpp")
EXE_MAIN = os.path.join(tempfile.gettempdir(), "P1655")
EXE_BRUTE = os.path.join(tempfile.gettempdir(), "P1655_brute")
CXX = ["g++", "-O2", "-std=c++17", "-Wall"]


def build():
    for src, out in ((SRC_MAIN, EXE_MAIN), (SRC_BRUTE, EXE_BRUTE)):
        r = subprocess.run(CXX + [src, "-o", out], capture_output=True, text=True)
        if r.returncode != 0 or r.stderr.strip():
            print("编译失败或有警告:", src, r.stderr)
            sys.exit(1)


def run(exe, inp):
    r = subprocess.run([exe], input=inp, capture_output=True, text=True)
    if r.returncode != 0:
        print("运行失败:", exe, r.stderr)
        sys.exit(1)
    return r.stdout.strip()


def main():
    build()
    # Test all pairs 1 <= n, m <= 25 against inclusion-exclusion brute
    lines = [f"{n} {m}" for n in range(1, 26) for m in range(1, 26)]
    inp = "\n".join(lines) + "\n"
    o1 = run(EXE_MAIN, inp)
    o2 = run(EXE_BRUTE, inp)
    assert o1 == o2, "Mismatch on 1<=n,m<=25!"
    # Worst-case timing
    big_lines = [f"100 {m}" for m in range(1, 101)]
    t0 = time.perf_counter()
    run(EXE_MAIN, "\n".join(big_lines) + "\n")
    dt = time.perf_counter() - t0
    print(f"P1655: all 625 cases (1<=n,m<=25) passed; 100 queries up to n=100 ran in {dt:.4f}s")


if __name__ == "__main__":
    main()
