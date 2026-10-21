#!/usr/bin/env python3
import os
import random
import subprocess
import sys
import tempfile
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1653.cpp")
SRC_BRUTE = os.path.join(BASE, "P1653_brute.cpp")
EXE_MAIN = os.path.join(tempfile.gettempdir(), "P1653")
EXE_BRUTE = os.path.join(tempfile.gettempdir(), "P1653_brute")
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
    w = rng.randint(1, 15)
    l = rng.randint(1, 15)
    hmax = rng.choice([0, 2, 5, 50])
    lines = [f"{w} {l}"]
    for _ in range(l):
        lines.append(" ".join(str(rng.randint(0, hmax)) for _ in range(w)))
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
    w, l = 500, 500
    lines = [f"{w} {l}"]
    for r in range(l):
        lines.append(" ".join(str((r * 17 + c * 31) % 100) for c in range(w)))
    t0 = time.perf_counter()
    run(EXE_MAIN, "\n".join(lines) + "\n")
    dt = time.perf_counter() - t0
    print(f"P1653: {ROUNDS} random cases passed; 500x500 ran in {dt:.4f}s")


if __name__ == "__main__":
    main()
