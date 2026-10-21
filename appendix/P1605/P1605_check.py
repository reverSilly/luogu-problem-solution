#!/usr/bin/env python3
# P1605 对拍: 标程(到达终点即截断的 DFS) vs 暴力(枚举全部自回避路径, 不截断)
# 编译:
#   g++ -O2 -std=c++17 -Wall -o /tmp/P1605 P1605.cpp
#   g++ -O2 -std=c++17 -Wall -o /tmp/P1605_brute P1605_brute.cpp
import random
import subprocess
import sys

STD = "/tmp/P1605"
BRU = "/tmp/P1605_brute"


def run(path, data):
    r = subprocess.run([path], input=data.encode(), capture_output=True)
    return r.stdout.decode().strip()


def main():
    rng = random.Random(20260521)
    cases = [(1, 1, 1, 1, 1, 1, []),                 # 起点即终点
             (5, 5, 1, 1, 5, 5, []),                # 无障碍满格
             (2, 2, 1, 1, 2, 2, []),
             (5, 1, 1, 1, 1, 5, []),                # 退化成一列
             (1, 5, 1, 1, 1, 1, [])]
    for _ in range(145):
        n = rng.randint(1, 4)
        m = rng.randint(1, 4)
        sx, sy = rng.randint(1, n), rng.randint(1, m)
        fx, fy = rng.randint(1, n), rng.randint(1, m)
        cells = [(i, j) for i in range(1, n + 1) for j in range(1, m + 1)
                 if (i, j) != (sx, sy)]
        rng.shuffle(cells)
        obs = cells[:rng.randint(0, min(10, len(cells)))]
        cases.append((n, m, sx, sy, fx, fy, obs))
    for idx, (n, m, sx, sy, fx, fy, obs) in enumerate(cases):
        lines = [f"{n} {m} {len(obs)}", f"{sx} {sy} {fx} {fy}"]
        lines += [f"{x} {y}" for x, y in obs]
        data = "\n".join(lines) + "\n"
        a, b = run(STD, data), run(BRU, data)
        if a != b:
            print("MISMATCH on test", idx)
            print(data)
            print("std:", a, "brute:", b)
            return 1
    print(f"{len(cases)} tests OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
