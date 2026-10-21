#!/usr/bin/env python3
import os
import random
import subprocess
import sys
import tempfile
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1651.cpp")
SRC_BRUTE = os.path.join(BASE, "P1651_brute.cpp")
EXE_MAIN = os.path.join(tempfile.gettempdir(), "P1651")
EXE_BRUTE = os.path.join(tempfile.gettempdir(), "P1651_brute")
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
    n = rng.randint(1, 13)
    h = [rng.randint(1, 100) for _ in range(n)]
    return f"{n}\n" + " ".join(map(str, h)) + "\n"


def main():
    build()
    assert run(EXE_MAIN, "3\n2 3 5\n") == "5"
    assert run(EXE_BRUTE, "3\n2 3 5\n") == "5"
    rng = random.Random(20261021)
    for it in range(ROUNDS):
        inp = gen(rng)
        o1 = run(EXE_MAIN, inp)
        o2 = run(EXE_BRUTE, inp)
        if o1 != o2:
            print(f"Mismatch at round {it+1}:\n{inp}\nstd: {o1}\nbrute: {o2}")
            sys.exit(1)
    n = 50
    h = [10000 for _ in range(n)]
    inp = f"{n}\n" + " ".join(map(str, h)) + "\n"
    t0 = time.perf_counter()
    run(EXE_MAIN, inp)
    dt = time.perf_counter() - t0
    print(f"P1651: {ROUNDS} random cases passed; n=50, sum=500000 ran in {dt:.4f}s")


if __name__ == "__main__":
    main()
