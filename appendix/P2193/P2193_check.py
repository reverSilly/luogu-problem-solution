#!/usr/bin/env python3
# P2193 对拍：标程 vs 暴力（暴力 DFS 枚举所有合法序列并计数）
import random, subprocess, os, sys, tempfile

BASE = os.path.dirname(os.path.abspath(__file__))


def build(tmp):
    std = os.path.join(tmp, "std")
    bru = os.path.join(tmp, "bru")
    subprocess.check_call(["g++", "-O2", "-std=c++17", os.path.join(BASE, "P2193.cpp"), "-o", std])
    subprocess.check_call(["g++", "-O2", "-std=c++17", os.path.join(BASE, "P2193_brute.cpp"), "-o", bru])
    return std, bru


def run(exe, data):
    r = subprocess.run([exe], input=data, capture_output=True, text=True, timeout=30)
    return r.stdout.strip()


def main():
    random.seed(20240602)
    with tempfile.TemporaryDirectory() as tmp:
        std, bru = build(tmp)
        cases = 0
        for it in range(2000):
            n = random.randint(1, 7)
            p = random.randint(1, 5)
            data = f"{n} {p}\n"
            x, y = run(std, data), run(bru, data)
            if x != y:
                print("MISMATCH at iter", it)
                print("input:", data)
                print("std:", x)
                print("brute:", y)
                return 1
            cases += 1
    print(f"P2193: {cases} cases, all matched")
    return 0


if __name__ == "__main__":
    sys.exit(main())
