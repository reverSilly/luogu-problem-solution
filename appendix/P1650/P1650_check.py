#!/usr/bin/env python3
import os
import random
import subprocess
import sys
import tempfile
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1650.cpp")
SRC_BRUTE = os.path.join(BASE, "P1650_brute.cpp")
EXE_MAIN = os.path.join(tempfile.gettempdir(), "P1650")
EXE_BRUTE = os.path.join(tempfile.gettempdir(), "P1650_brute")
CXX = ["g++", "-O2", "-std=c++17", "-Wall"]
ROUNDS = 600


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
    n = rng.randint(1, 50)
    vmax = rng.choice([3, 10, 100])
    a = [rng.randint(0, vmax) for _ in range(n)]
    b = [rng.randint(0, vmax) for _ in range(n)]
    return f"{n}\n" + " ".join(map(str, a)) + "\n" + " ".join(map(str, b)) + "\n"


def main():
    build()
    sample = "3\n92 83 71\n95 87 74\n"
    assert run(EXE_MAIN, sample) == "200"
    assert run(EXE_BRUTE, sample) == "200"
    rng = random.Random(20261021)
    for it in range(ROUNDS):
        inp = gen(rng)
        o1 = run(EXE_MAIN, inp)
        o2 = run(EXE_BRUTE, inp)
        if o1 != o2:
            print(f"Mismatch at round {it+1}:\n{inp}\nstd: {o1}\nbrute: {o2}")
            sys.exit(1)
    n = 2000
    a = [rng.randint(0, 100) for _ in range(n)]
    b = [rng.randint(0, 100) for _ in range(n)]
    inp = f"{n}\n" + " ".join(map(str, a)) + "\n" + " ".join(map(str, b)) + "\n"
    t0 = time.perf_counter()
    run(EXE_MAIN, inp)
    dt = time.perf_counter() - t0
    print(f"P1650: {ROUNDS} random cases passed; n=2000 ran in {dt:.4f}s")


if __name__ == "__main__":
    main()
