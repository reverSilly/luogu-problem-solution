#!/usr/bin/env python3
"""P1522 [USACO2.4] 牛的旅行 Cow Tours 对拍与验证脚本"""
import os
import random
import subprocess
import sys
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1522.cpp")
SRC_BRUTE = os.path.join(BASE, "P1522_brute.cpp")
EXE_MAIN = os.path.join(BASE, "P1522")
EXE_BRUTE = os.path.join(BASE, "P1522_brute")
CXX = ["g++", "-O2", "-std=c++17"]
ROUNDS = 1000

SAMPLE_IN = """8
10 10
15 10
20 10
15 15
20 15
30 15
25 10
30 10
01000000
10111000
01001000
01001000
01110000
00000010
00000101
00000010
"""
SAMPLE_OUT = "22.071068"


def build():
    for src, out in ((SRC_MAIN, EXE_MAIN), (SRC_BRUTE, EXE_BRUTE)):
        r = subprocess.run(CXX + [src, "-o", out], capture_output=True, text=True)
        if r.returncode != 0:
            print("编译失败:", src)
            print(r.stderr)
            sys.exit(1)
        print("编译成功:", os.path.basename(out))


def run(exe, inp):
    r = subprocess.run([exe], input=inp, capture_output=True, text=True)
    if r.returncode != 0:
        print("运行失败:", exe)
        print(r.stderr)
        sys.exit(1)
    return r.stdout.strip()


def sample_case():
    gm = run(EXE_MAIN, SAMPLE_IN)
    gb = run(EXE_BRUTE, SAMPLE_IN)
    assert gm == SAMPLE_OUT and gb == SAMPLE_OUT, f"样例失败: gm={gm}, gb={gb}, exp={SAMPLE_OUT}"
    print(f"官方样例通过 -> {gm}")


def gen(rng):
    n = rng.randint(2, 16)
    pts = set()
    while len(pts) < n:
        pts.add((rng.randint(0, 200), rng.randint(0, 200)))
    pts = list(pts)
    # 划分为 2..4 个连通分量, 保证至少有两个不连通分量
    k = rng.randint(2, min(4, n))
    comp = [i % k for i in range(n)]
    rng.shuffle(comp)
    adj = [["0"] * n for _ in range(n)]
    for i in range(n):
        for j in range(i + 1, n):
            if comp[i] == comp[j] and rng.random() < 0.5:
                adj[i][j] = adj[j][i] = "1"
    lines = [str(n)] + [f"{x} {y}" for x, y in pts] + ["".join(row) for row in adj]
    return "\n".join(lines) + "\n"


def stress():
    rng = random.Random(20261016)
    for it in range(ROUNDS):
        inp = gen(rng)
        gm = run(EXE_MAIN, inp)
        gb = run(EXE_BRUTE, inp)
        if gm != gb:
            print(f"对拍不一致! 第 {it+1} 组:\n{inp}标程: {gm} | 暴力: {gb}")
            sys.exit(1)
    print(f"随机对拍: {ROUNDS} 组全部一致")


def boundary_cases():
    # 关键反例: 原连通块内部直径(100) 大于 跨桥路径长度(1+1+1=3), 漏取 max(diam[c1], diam[c2]) 会错报 3
    case1 = """4
0 0
100 0
50 0
50 1
0110
1010
1100
0000
"""
    # 在此图中 {0,1,2} 连通: 0=(0,0), 2=(50,0), 1=(100,0), 若只有 0-2 和 2-1 连边, 直径为 100, 而 mxd[2]=50;
    # 更极端: 设 {0,1,2} 为星形, 中心 2=(50,0) 连 (50,10) 和 (50,-10) 之外再拉长链, 或直接检验标程与暴力
    gm = run(EXE_MAIN, case1)
    gb = run(EXE_BRUTE, case1)
    assert gm == gb, f"边界失败: gm={gm}, gb={gb}"
    print(f"边界通过: 内部直径主导测试 -> {gm}")


def worst_case_timing():
    n = 150
    lines = [str(n)] + [f"{i * 10} {(i * 37) % 1000}" for i in range(n)]
    for i in range(n):
        row = ["1" if (j != i and (i < 75) == (j < 75) and abs(i - j) <= 3) else "0" for j in range(n)]
        lines.append("".join(row))
    inp = "\n".join(lines) + "\n"
    best = None
    out = None
    for _ in range(5):
        t0 = time.perf_counter()
        out = run(EXE_MAIN, inp)
        dt = time.perf_counter() - t0
        best = dt if best is None else min(best, dt)
    print(f"最坏规模 (N=150): 输出 {out}, 含进程启动最快 {best:.4f} s")


def main():
    build()
    sample_case()
    boundary_cases()
    stress()
    worst_case_timing()
    print("全部验证通过")


if __name__ == "__main__":
    main()
