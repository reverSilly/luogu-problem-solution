#!/usr/bin/env python3
import os
import random
import subprocess
import sys
import tempfile
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1642.cpp")
SRC_BRUTE = os.path.join(BASE, "P1642_brute.cpp")
EXE_MAIN = os.path.join(tempfile.gettempdir(), "P1642")
EXE_BRUTE = os.path.join(tempfile.gettempdir(), "P1642_brute")
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
    n = rng.randint(2, 12)
    m = rng.randint(1, n - 1)
    w = [rng.randint(1, 10000) for _ in range(n)]
    c = [rng.randint(1, 10000) for _ in range(n)]
    edges = [f"{i+1} {rng.randint(1, i)}" for i in range(1, n)]
    return f"{n} {m}\n" + " ".join(map(str, w)) + "\n" + " ".join(map(str, c)) + "\n" + "\n".join(edges) + "\n"


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
    n, m = 99, 45
    w = [rng.randint(1, 10000) for _ in range(n)]
    c = [rng.randint(1, 10000) for _ in range(n)]
    edges = [f"{i+1} {rng.randint(1, i)}" for i in range(1, n)]
    inp = f"{n} {m}\n" + " ".join(map(str, w)) + "\n" + " ".join(map(str, c)) + "\n" + "\n".join(edges) + "\n"
    t0 = time.perf_counter()
    run(EXE_MAIN, inp)
    dt = time.perf_counter() - t0
    print(f"P1642: {ROUNDS} random cases passed; n=99 ran in {dt:.4f}s")


if __name__ == "__main__":
    main()
