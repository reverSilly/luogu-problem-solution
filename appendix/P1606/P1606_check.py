#!/usr/bin/env python3
# P1606 对拍: 标程(0-1 BFS + 按距离分层"按集合"计数) vs 暴力(枚举水格子集)
# 编译:
#   g++ -O2 -std=c++17 -Wall -o /tmp/P1606 P1606.cpp
#   g++ -O2 -std=c++17 -Wall -o /tmp/P1606_brute P1606_brute.cpp
import random
import subprocess
import sys

STD = "/tmp/P1606"
BRU = "/tmp/P1606_brute"


def run(path, data):
    r = subprocess.run([path], input=data.encode(), capture_output=True)
    return r.stdout.decode().strip()


def main():
    rng = random.Random(20260521)
    cases = [
        # 题面样例: 最少 2 片, 3 种放法
        "4 5\n1 0 0 0 0\n3 0 0 0 0\n0 0 2 0 0\n0 0 0 4 0\n",
        # 同一片荷叶可由两条不同路径到达: 数路径会得 2, 正确答案是 1
        "4 5\n0 1 2 0 1\n1 0 2 1 0\n0 0 0 1 0\n2 3 0 0 4\n",
        # 起点即终点相邻的退化情形
        "1 2\n3 4\n",
        "2 2\n3 0\n0 4\n",
    ]
    for _ in range(160):                       # 小网格, 大量水
        M = rng.randint(1, 4)
        N = rng.randint(1, 4)
        while M * N < 2:
            M = rng.randint(1, 4)
            N = rng.randint(1, 4)
        cells = [(i, j) for i in range(M) for j in range(N)]
        s, t = rng.sample(cells, 2)
        g = {c: rng.choices([0, 1, 2], weights=[7, 2, 1])[0] for c in cells}
        g[s] = 3
        g[t] = 4
        lines = [f"{M} {N}"]
        for i in range(M):
            lines.append(" ".join(str(g[(i, j)]) for j in range(N)))
        cases.append("\n".join(lines) + "\n")
    for _ in range(40):                        # 5x5, 多放岩石把水格数压到 <=17
        M = N = 5
        cells = [(i, j) for i in range(M) for j in range(N)]
        s, t = rng.sample(cells, 2)
        g = {c: rng.choices([0, 1, 2], weights=[4, 3, 3])[0] for c in cells}
        g[s] = 3
        g[t] = 4
        if sum(1 for v in g.values() if v == 0) > 17:
            continue
        lines = [f"{M} {N}"]
        for i in range(M):
            lines.append(" ".join(str(g[(i, j)]) for j in range(N)))
        cases.append("\n".join(lines) + "\n")

    tested = 0
    for idx, data in enumerate(cases):
        a, b = run(STD, data), run(BRU, data)
        if b.startswith("TOOBIG"):
            continue
        tested += 1
        if a != b:
            print("MISMATCH on test", idx)
            print(data)
            print("std:", a, "brute:", b)
            return 1
    print(f"{tested} tests OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
