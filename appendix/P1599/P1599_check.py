#!/usr/bin/env python3
# P1599 对拍: 标程(前缀和公式) vs 暴力(状态 Dijkstra)
# 编译:
#   g++ -O2 -std=c++17 -Wall -o /tmp/P1599 P1599.cpp
#   g++ -O2 -std=c++17 -Wall -o /tmp/P1599_brute P1599_brute.cpp
import random
import subprocess
import sys

STD = "/tmp/P1599"
BRU = "/tmp/P1599_brute"


def run(path, data):
    r = subprocess.run([path], input=data.encode(), capture_output=True)
    return r.stdout.decode().strip()


def gen(rng):
    while True:
        n = rng.randint(1, 7)
        d = [rng.choice([x for x in range(-6, 7) if x != 0]) for _ in range(n)]
        if sum(d) > 0:                      # 题面保证朋友欠她的更多
            return n, d


def main():
    rng = random.Random(20260521)
    T = 200
    for t in range(T):
        n, d = gen(rng)
        data = str(n) + "\n" + "\n".join(map(str, d)) + "\n"
        a = run(STD, data)
        b = run(BRU, data)
        if a != b:
            print("MISMATCH on test", t)
            print(data)
            print("std:", a, "brute:", b)
            return 1
    print(f"{T} tests OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
