#!/usr/bin/env python3
import os
import subprocess
import sys
import tempfile
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1647.cpp")
SRC_BRUTE = os.path.join(BASE, "P1647_brute.cpp")
EXE_MAIN = os.path.join(tempfile.gettempdir(), "P1647")
EXE_BRUTE = os.path.join(tempfile.gettempdir(), "P1647_brute")
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
    assert run(EXE_MAIN, "3 8\n") == "111"
    assert run(EXE_BRUTE, "3 8\n") == "111"
    total = 0
    for n in range(1, 10):
        for k in range(1, (1 << n) + 1):
            inp = f"{n} {k}\n"
            o1 = run(EXE_MAIN, inp)
            o2 = run(EXE_BRUTE, inp)
            if o1 != o2:
                print(f"Mismatch at n={n}, k={k}: std={o1}, brute={o2}")
                sys.exit(1)
            total += 1
    t0 = time.perf_counter()
    out50 = run(EXE_MAIN, f"50 {(1 << 49) + 123456789}\n")
    dt = time.perf_counter() - t0
    assert len(out50) == 50
    print(f"P1647: all {total} exhaustive (N<=9) cases passed; N=50 ran in {dt:.4f}s")


if __name__ == "__main__":
    main()
