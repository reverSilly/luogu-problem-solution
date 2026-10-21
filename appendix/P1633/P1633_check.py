#!/usr/bin/env python3
import os
import random
import subprocess
import sys
import tempfile
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1633.cpp")
SRC_BRUTE = os.path.join(BASE, "P1633_brute.cpp")
EXE_MAIN = os.path.join(tempfile.gettempdir(), "P1633")
EXE_BRUTE = os.path.join(tempfile.gettempdir(), "P1633_brute")
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
    t = rng.randint(1, 5)
    lines = [str(t)]
    for _ in range(t):
        a = rng.randint(1, 255)
        b = rng.randint(1, 255)
        c = rng.randint(1, 255)
        lines.append(f"{a} {b} {c}")
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
    lines = ["10"]
    for _ in range(10):
        a = rng.randint(1 << 29, 1 << 30)
        b = rng.randint(1 << 29, 1 << 30)
        c = rng.randint(1 << 29, 1 << 30)
        lines.append(f"{a} {b} {c}")
    t0 = time.perf_counter()
    run(EXE_MAIN, "\n".join(lines) + "\n")
    dt = time.perf_counter() - t0
    print(f"P1633: {ROUNDS} random batches passed; T=10 (A,B,C ~ 2^30) ran in {dt:.4f}s")


if __name__ == "__main__":
    main()
