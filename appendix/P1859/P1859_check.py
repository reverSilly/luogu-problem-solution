#!/usr/bin/env python3
# P1859 对拍：滚动 DP vs 深搜枚举拒绝集合
import random
import subprocess
import sys
import time

STD_SRC = "/tmp/luogu-ps/appendix/P1859/P1859.cpp"
BRU_SRC = "/tmp/luogu-ps/appendix/P1859/P1859_brute.cpp"
STD = "/tmp/P1859"
BRU = "/tmp/P1859_brute"

CMDS = ["FORWARD", "BACK", "LEFT", "RIGHT"]

def compile_all():
    for src, out in ((STD_SRC, STD), (BRU_SRC, BRU)):
        r = subprocess.run(
            ["g++", "-O2", "-std=c++17", "-Wall", "-Wextra", "-o", out, src],
            capture_output=True, text=True)
        if r.returncode != 0 or r.stderr.strip():
            print("COMPILE FAIL", src)
            print(r.stderr)
            return 1
    return 0

def run(path, data, timeout=20):
    r = subprocess.run([path], input=data.encode(), capture_output=True, timeout=timeout)
    if r.returncode != 0:
        raise RuntimeError(r.stderr.decode())
    return r.stdout.decode().strip()

def grid_case(m, n, sx, sy, cells, cmds):
    lines = [f"{m} {n} {sx} {sy}"]
    for i in range(m):
        lines.append("".join(cells[i]))
    lines.extend(cmds)
    return "\n".join(lines) + "\n"

def main():
    if compile_all():
        return 1
    rng = random.Random(20261021)
    cases = []
    cases.append("""4 7 3 3
.***
..**
*..*
****
LEFT
FORWARD
LEFT
BACK
FORWARD
RIGHT
FORWARD
""")
    cases.append(grid_case(1, 0, 1, 1, [["."]], []))
    cases.append(grid_case(1, 1, 1, 1, [["."]], ["FORWARD"]))
    cases.append(grid_case(1, 2, 1, 1, [["."]], ["LEFT", "RIGHT"]))
    cases.append(grid_case(2, 1, 2, 1, [[".", "."], [".", "."]], ["FORWARD"]))
    cases.append(grid_case(2, 2, 2, 2, [[".", "."], [".", "."]], ["RIGHT", "FORWARD"]))
    cases.append(grid_case(3, 4, 2, 2, [[".", ".", "."], [".", "*", "."], [".", ".", "."]],
                           ["FORWARD", "LEFT", "FORWARD", "BACK"]))
    for _ in range(60):
        m = rng.randint(1, 6)
        n = rng.randint(0, 12)
        cells = [[rng.choice([".", ".", ".", "*"]) for _ in range(m)] for _ in range(m)]
        sx = rng.randint(1, m)
        sy = rng.randint(1, m)
        cells[sx - 1][sy - 1] = "."
        cmds = [rng.choice(CMDS) for _ in range(n)]
        cases.append(grid_case(m, n, sx, sy, cells, cmds))
    for idx, data in enumerate(cases):
        so = run(STD, data)
        bo = run(BRU, data)
        if so != bo:
            print("MISMATCH", idx)
            print(data)
            print("std", so, "brute", bo)
            return 1
    # 最大规模
    m, n = 100, 1000
    cells = [["." for _ in range(m)] for _ in range(m)]
    rng2 = random.Random(3)
    for _k in range(800):
        cells[rng2.randint(0, m - 1)][rng2.randint(0, m - 1)] = "*"
    cells[49][49] = "."
    cmds = [rng2.choice(CMDS) for _ in range(n)]
    big = grid_case(m, n, 50, 50, cells, cmds)
    t0 = time.perf_counter()
    so = run(STD, big, timeout=10)
    dt = time.perf_counter() - t0
    if not so.isdigit() or int(so) < 0 or int(so) > n:
        print("BAD MAX", so)
        return 1
    print(f"max M=100 N=1000 time={dt:.6f}s out={so}")
    print(f"{len(cases)} tests OK")
    return 0

if __name__ == "__main__":
    sys.exit(main())
