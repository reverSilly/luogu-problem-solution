#!/usr/bin/env python3
import os
import random
import subprocess
import sys
import tempfile
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1648.cpp")
SRC_BRUTE = os.path.join(BASE, "P1648_brute.cpp")
EXE_MAIN = os.path.join(tempfile.gettempdir(), "P1648")
EXE_BRUTE = os.path.join(tempfile.gettempdir(), "P1648_brute")
CXX = ["g++", "-O2", "-std=c++17", "-Wall"]
ROUNDS = 500


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
    n = rng.randint(2, 60)
    d = rng.randint(1, 4)
    lines = [f"{n} {d}"]
    for _ in range(n):
        lines.append(" ".join(str(rng.randint(1, 100000)) for _ in range(d)))
    return "\n".join(lines) + "\n"


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
    n, d = 200000, 4
    lines = [f"{n} {d}"] + [f"{(i*17)%100000+1} {(i*31)%100000+1} {(i*47)%100000+1} {(i*61)%100000+1}" for i in range(n)]
    t0 = time.perf_counter()
    run(EXE_MAIN, "\n".join(lines) + "\n")
    dt = time.perf_counter() - t0
    print(f"P1648: {ROUNDS} random cases passed; n=200000, d=4 ran in {dt:.4f}s")


if __name__ == "__main__":
    main()
