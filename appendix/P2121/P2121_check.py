#!/usr/bin/env python3
# P2121 对拍：检验降序 Kruskal（至多 K 条且不成环，权值和最大）
import random
import subprocess
import sys

STD = "/tmp/P2121"
BRUTE = "/tmp/P2121_brute"


def run(prog, data):
    r = subprocess.run([prog], input=data, capture_output=True, text=True, timeout=30)
    return r.stdout.strip()


def main():
    random.seed(21211)
    cases = 0
    for _ in range(300):
        n = random.randint(2, 8)
        m = random.randint(1, 12)
        K = random.randint(1, 6)
        edges = []
        for _ in range(m):
            u = random.randint(1, n)
            v = random.randint(1, n)
            if u == v:
                v = (v % n) + 1
            w = random.randint(1, 30)
            edges.append((u, v, w))
        data = f"{n} {m} {K}\n" + "".join(f"{u} {v} {w}\n" for u, v, w in edges)
        x = run(STD, data)
        y = run(BRUTE, data)
        cases += 1
        if x != y:
            print("MISMATCH data:")
            print(data)
            print("std:", x, "brute:", y)
            return 1
    print(f"all {cases} cases match")
    return 0


if __name__ == "__main__":
    sys.exit(main())
