#!/usr/bin/env python3
# P1604 对拍: 标程(逐位模拟) vs 暴力(cpp_int 转十进制再转回)
# 编译:
#   g++ -O2 -std=c++17 -Wall -o /tmp/P1604 P1604.cpp
#   g++ -O2 -std=c++17 -Wall -o /tmp/P1604_brute P1604_brute.cpp
import random
import subprocess
import sys

STD = "/tmp/P1604"
BRU = "/tmp/P1604_brute"
DIG = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"


def run(path, data):
    r = subprocess.run([path], input=data.encode(), capture_output=True)
    return r.stdout.decode().strip()


def num(rng, B):
    L = rng.choice([1, 1, 2, 3, 10, 100, 1999, 2000])
    s = "".join(DIG[rng.randint(0, B - 1)] for _ in range(L))
    if rng.random() < 0.15:
        s = "0" * rng.randint(1, 3) + s
    return s


def main():
    rng = random.Random(20260521)
    cases = [(2, "1", "1"), (36, "Z", "1"), (10, "0", "0"),
             (16, "F" * 2000, "1"), (2, "1" * 2000, "1" * 2000),
             (36, "Z" * 2000, "Z" * 2000), (8, "7", "7")]
    for _ in range(160):
        B = rng.randint(2, 36)
        cases.append((B, num(rng, B), num(rng, B)))
    for t, (B, a, b) in enumerate(cases):
        data = f"{B}\n{a}\n{b}\n"
        x, y = run(STD, data), run(BRU, data)
        if x != y:
            print("MISMATCH on test", t, "B=", B, a[:20], b[:20])
            print("std:", x[:40], "brute:", y[:40])
            return 1
    print(f"{len(cases)} tests OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
