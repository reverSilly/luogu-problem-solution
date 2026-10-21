#!/usr/bin/env python3
import os
import random
import subprocess
import sys
import tempfile
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1659.cpp")
SRC_BRUTE = os.path.join(BASE, "P1659_brute.cpp")
EXE_MAIN = os.path.join(tempfile.gettempdir(), "P1659")
EXE_BRUTE = os.path.join(tempfile.gettempdir(), "P1659_brute")
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
    n = rng.randint(1, 60)
    alpha = rng.choice(["ab", "abc", "a", "abcd"])
    s = "".join(rng.choice(alpha) for _ in range(n))
    k = rng.randint(1, n * (n + 1) // 2 + 5)
    return f"{n} {k}\n{s}\n"


def main():
    build()
    sample = "5 3\nababa\n"
    assert run(EXE_MAIN, sample) == "45"
    assert run(EXE_BRUTE, sample) == "45"
    rng = random.Random(20261021)
    for it in range(ROUNDS):
        inp = gen(rng)
        o1 = run(EXE_MAIN, inp)
        o2 = run(EXE_BRUTE, inp)
        if o1 != o2:
            print(f"Mismatch at round {it+1}:\n{inp}\nstd: {o1}\nbrute: {o2}")
            sys.exit(1)
    n = 1000000
    s = "ab" * (n // 2)
    inp = f"{n} 100000000000\n{s}\n"
    t0 = time.perf_counter()
    run(EXE_MAIN, inp)
    dt = time.perf_counter() - t0
    print(f"P1659: {ROUNDS} random cases passed; n=1000000 ran in {dt:.4f}s")


if __name__ == "__main__":
    main()
