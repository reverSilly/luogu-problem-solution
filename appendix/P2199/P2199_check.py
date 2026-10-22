#!/usr/bin/env python3
# P2199 对拍：标程 vs 暴力
# 暴力：逐格判断是否与奖杯同行/列/对角且中间无墙，再 BFS 求哈利到最近可见格的最短步数
import random, subprocess, os, sys, tempfile

BASE = os.path.dirname(os.path.abspath(__file__))


def build(tmp):
    std = os.path.join(tmp, "std")
    bru = os.path.join(tmp, "bru")
    subprocess.check_call(["g++", "-O2", "-std=c++17", os.path.join(BASE, "P2199.cpp"), "-o", std])
    subprocess.check_call(["g++", "-O2", "-std=c++17", os.path.join(BASE, "P2199_brute.cpp"), "-o", bru])
    return std, bru


def run(exe, data):
    r = subprocess.run([exe], input=data, capture_output=True, text=True, timeout=30)
    return r.stdout.strip()


def main():
    random.seed(20240605)
    with tempfile.TemporaryDirectory() as tmp:
        std, bru = build(tmp)
        cases = 0
        for it in range(2000):
            n = random.randint(1, 6)
            m = random.randint(1, 6)
            grid = [["O" if random.random() < 0.75 else "X" for _ in range(m)] for _ in range(n)]
            # 保证至少有 4 个空地作为查询点
            free = [(i, j) for i in range(n) for j in range(m) if grid[i][j] == "O"]
            if len(free) < 4:
                for i in range(n):
                    for j in range(m):
                        grid[i][j] = "O"
                free = [(i, j) for i in range(n) for j in range(m)]
            data = f"{n} {m}\n" + "".join("".join(r) + "\n" for r in grid)
            q = random.randint(1, 4)
            for _ in range(q):
                (ex, ey) = random.choice(free)
                (sx, sy) = random.choice(free)
                data += f"{ex+1} {ey+1} {sx+1} {sy+1}\n"
            data += "0 0 0 0\n"
            x, y = run(std, data), run(bru, data)
            if x != y:
                print("MISMATCH at iter", it)
                print("input:\n" + data)
                print("std:", x)
                print("brute:", y)
                return 1
            cases += 1
    print(f"P2199: {cases} cases, all matched")
    return 0


if __name__ == "__main__":
    sys.exit(main())
