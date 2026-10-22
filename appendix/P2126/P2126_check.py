#!/usr/bin/env python3
# P2126 对拍：检验邻接矩阵 Prim（重边按“后读入覆盖”处理）
import random
import subprocess
import sys

STD = "/tmp/P2126"
BRUTE = "/tmp/P2126_brute"


def run(prog, data):
    r = subprocess.run([prog], input=data, capture_output=True, text=True, timeout=30)
    return r.stdout.strip()


def main():
    random.seed(21261)
    cases = 0
    for _ in range(300):
        n = random.randint(1, 8)
        tot = n + 1  # 0 号是 mzc
        m = random.randint(tot - 1, 3 * tot)
        edges = []
        for _ in range(m):
            a = random.randint(0, n)
            b = random.randint(0, n)
            if a == b:
                b = (b + 1) % tot
            w = random.randint(1, 50)
            edges.append((a, b, w))
        data = f"{n}\n{m}\n" + "".join(f"{a} {b} {w}\n" for a, b, w in edges)
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
