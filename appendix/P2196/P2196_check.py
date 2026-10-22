#!/usr/bin/env python3
# P2196 对拍：标程 vs 暴力（暴力 DFS 枚举所有合法路径，取最大地雷数与字典序最小路径）
import random, subprocess, os, sys, tempfile

BASE = os.path.dirname(os.path.abspath(__file__))


def build(tmp):
    std = os.path.join(tmp, "std")
    bru = os.path.join(tmp, "bru")
    subprocess.check_call(["g++", "-O2", "-std=c++17", os.path.join(BASE, "P2196.cpp"), "-o", std])
    subprocess.check_call(["g++", "-O2", "-std=c++17", os.path.join(BASE, "P2196_brute.cpp"), "-o", bru])
    return std, bru


def run(exe, data):
    r = subprocess.run([exe], input=data, capture_output=True, text=True, timeout=30)
    return r.stdout.strip()


def main():
    random.seed(20240604)
    with tempfile.TemporaryDirectory() as tmp:
        std, bru = build(tmp)
        cases = 0
        for it in range(2000):
            n = random.randint(1, 7)
            a = [random.randint(0, 300) for _ in range(n)]
            # 上三角随机连通矩阵
            rows = []
            for i in range(n - 1):
                rows.append([random.randint(0, 1) for _ in range(n - i - 1)])
            data = f"{n}\n" + " ".join(map(str, a)) + "\n"
            for r in rows:
                data += " ".join(map(str, r)) + "\n"
            x, y = run(std, data), run(bru, data)
            if x != y:
                print("MISMATCH at iter", it)
                print("input:\n" + data)
                print("std:", x)
                print("brute:", y)
                return 1
            cases += 1
    print(f"P2196: {cases} cases, all matched")
    return 0


if __name__ == "__main__":
    sys.exit(main())
