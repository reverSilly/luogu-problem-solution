#!/usr/bin/env python3
# P2194 对拍：标程 vs 暴力（暴力用传递闭包求强连通分量）
import random, subprocess, os, sys, tempfile

BASE = os.path.dirname(os.path.abspath(__file__))


def build(tmp):
    std = os.path.join(tmp, "std")
    bru = os.path.join(tmp, "bru")
    subprocess.check_call(["g++", "-O2", "-std=c++17", os.path.join(BASE, "P2194.cpp"), "-o", std])
    subprocess.check_call(["g++", "-O2", "-std=c++17", os.path.join(BASE, "P2194_brute.cpp"), "-o", bru])
    return std, bru


def run(exe, data):
    r = subprocess.run([exe], input=data, capture_output=True, text=True, timeout=30)
    return r.stdout.strip()


def main():
    random.seed(20240603)
    with tempfile.TemporaryDirectory() as tmp:
        std, bru = build(tmp)
        cases = 0
        for it in range(2000):
            n = random.randint(1, 8)
            w = [random.randint(0, 20) for _ in range(n)]
            m = random.randint(1, 12)
            edges = []
            for _ in range(m):
                x = random.randint(1, n)
                y = random.randint(1, n)
                edges.append((x, y))
            data = f"{n}\n" + " ".join(map(str, w)) + f"\n{m}\n"
            data += "".join(f"{x} {y}\n" for x, y in edges)
            x, y = run(std, data), run(bru, data)
            if x != y:
                print("MISMATCH at iter", it)
                print("input:\n" + data)
                print("std:", x)
                print("brute:", y)
                return 1
            cases += 1
    print(f"P2194: {cases} cases, all matched")
    return 0


if __name__ == "__main__":
    sys.exit(main())
