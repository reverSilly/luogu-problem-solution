#!/usr/bin/env python3
# P1601 对拍: 标程(逐位模拟) vs 暴力(cpp_int)
# 编译:
#   g++ -O2 -std=c++17 -Wall -o /tmp/P1601 P1601.cpp
#   g++ -O2 -std=c++17 -Wall -o /tmp/P1601_brute P1601_brute.cpp
import random
import subprocess
import sys

STD = "/tmp/P1601"
BRU = "/tmp/P1601_brute"


def run(path, data):
    r = subprocess.run([path], input=data.encode(), capture_output=True)
    return r.stdout.decode().strip()


def num(rng):
    L = rng.choice([1, 1, 2, 3, 5, 20, 100, 499, 500])
    s = "".join(str(rng.randint(0, 9)) for _ in range(L))
    if rng.random() < 0.15:                       # 少量带前导零的边界数据
        s = "0" * rng.randint(1, 3) + s
    return s


def main():
    rng = random.Random(20260521)
    cases = [("0", "0"), ("0", "1"), ("1", "0"), ("9", "1"),
             ("0" * 500, "0" * 500), ("9" * 500, "1"),
             ("1" + "0" * 499, "9" * 500)]
    T = 160
    for _ in range(T):
        cases.append((num(rng), num(rng)))
    for t, (a, b) in enumerate(cases):
        data = a + "\n" + b + "\n"
        x, y = run(STD, data), run(BRU, data)
        if x != y:
            print("MISMATCH on test", t, a[:30], b[:30])
            print("std:", x[:60], "brute:", y[:60])
            return 1
        if x != str(int(a) + int(b)):
            print("BAD expected on test", t)
            return 1
    print(f"{len(cases)} tests OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
