#!/usr/bin/env python3
import os
import random
import subprocess
import sys
import tempfile
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1641.cpp")
SRC_BRUTE = os.path.join(BASE, "P1641_brute.cpp")
EXE_MAIN = os.path.join(tempfile.gettempdir(), "P1641")
EXE_BRUTE = os.path.join(tempfile.gettempdir(), "P1641_brute")
CXX = ["g++", "-O2", "-std=c++17", "-Wall"]
ROUNDS = 400


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
    assert run(EXE_MAIN, "2 2\n") == "2"
    assert run(EXE_BRUTE, "2 2\n") == "2"
    rng = random.Random(20261021)
    for it in range(ROUNDS):
        n = rng.randint(1, 200)
        m = rng.randint(0, n + 10)
        inp = f"{n} {m}\n"
        o1 = run(EXE_MAIN, inp)
        o2 = run(EXE_BRUTE, inp)
        if o1 != o2:
            print(f"Mismatch at round {it+1}: {inp} -> std={o1}, brute={o2}")
            sys.exit(1)
    t0 = time.perf_counter()
    run(EXE_MAIN, "1000000 1000000\n")
    dt = time.perf_counter() - t0
    print(f"P1641: {ROUNDS} random cases passed; n=m=1000000 ran in {dt:.4f}s")


if __name__ == "__main__":
    main()
