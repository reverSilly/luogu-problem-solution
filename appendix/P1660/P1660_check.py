#!/usr/bin/env python3
import os
import random
import subprocess
import sys
import tempfile
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1660.cpp")
SRC_BRUTE = os.path.join(BASE, "P1660_brute.cpp")
EXE_MAIN = os.path.join(tempfile.gettempdir(), "P1660")
EXE_BRUTE = os.path.join(tempfile.gettempdir(), "P1660_brute")
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


def gen(rng):
    k = rng.randint(1, 6)
    a = rng.randint(1, 100000)
    b = a + rng.randint(0, 200)
    return f"{k} {a} {b}\n"


def main():
    build()
    rng = random.Random(20261021)
    for it in range(ROUNDS):
        inp = gen(rng)
        o1 = run(EXE_MAIN, inp)
        o2 = run(EXE_BRUTE, inp)
        if o1 != o2:
            print(f"Mismatch at round {it+1}:\n{inp}\nstd: {o1}\nbrute: {o2}")
            sys.exit(1)
    t0 = time.perf_counter()
    run(EXE_MAIN, "6 1 1000000\n")
    dt = time.perf_counter() - t0
    print(f"P1660: {ROUNDS} random cases passed; k=6, [1, 1000000] ran in {dt:.4f}s")


if __name__ == "__main__":
    main()
