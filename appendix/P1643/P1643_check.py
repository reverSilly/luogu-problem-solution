#!/usr/bin/env python3
import os
import random
import subprocess
import sys
import tempfile
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1643.cpp")
SRC_BRUTE = os.path.join(BASE, "P1643_brute.cpp")
EXE_MAIN = os.path.join(tempfile.gettempdir(), "P1643")
EXE_BRUTE = os.path.join(tempfile.gettempdir(), "P1643_brute")
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
    t = 5
    lines = [str(t)]
    for _ in range(t):
        x = rng.choice([0, 1, 9, 10, 99, 100, rng.randint(0, 50000)])
        n = rng.randint(1, 500)
        lines.append(str(x))
        lines.append(str(n))
    return "\n".join(lines) + "\n"


def main():
    build()
    sample = "2\n1\n1\n17\n2\n"
    assert run(EXE_MAIN, sample) == "2\n33"
    assert run(EXE_BRUTE, sample) == "2\n33"
    rng = random.Random(20261021)
    for it in range(ROUNDS):
        inp = gen(rng)
        o1 = run(EXE_MAIN, inp)
        o2 = run(EXE_BRUTE, inp)
        if o1 != o2:
            print(f"Mismatch at round {it+1}:\n{inp}\nstd:\n{o1}\nbrute:\n{o2}")
            sys.exit(1)
    big_x = "9" * 10000
    big_n = "9" * 10000
    lines = ["10"] + [f"{big_x}\n{big_n}" for _ in range(10)]
    t0 = time.perf_counter()
    run(EXE_MAIN, "\n".join(lines) + "\n")
    dt = time.perf_counter() - t0
    print(f"P1643: {ROUNDS} random batches passed; T=10 (10000-digit X,N) ran in {dt:.4f}s")


if __name__ == "__main__":
    main()
