#!/usr/bin/env python3
# P1510 精卫填海 对拍脚本
#   1) 自动编译 标程 + 暴力
#   2) 官方两组样例回归
#   3) 400 组随机小规模对拍(含 v=0 / 体力=0 / 体积=0 等边界)
#   4) 最坏规模计时 + 可判定大用例
# 任何一组不一致: 打印该组输入与双方输出, 并 sys.exit(1)
import os
import random
import subprocess
import sys
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1510.cpp")
SRC_BRUTE = os.path.join(BASE, "P1510_brute.cpp")
EXE_MAIN = os.path.join(BASE, "P1510")
EXE_BRUTE = os.path.join(BASE, "P1510_brute")
CXX = ["g++", "-O2", "-std=c++17"]
ROUNDS = 400

# 官方样例(取自洛谷原题页 P1510)
SAMPLES = [
    ("样例1", "100 2 10\n50 5\n50 5\n", "0"),
    ("样例2", "10 2 1\n50 5\n10 2\n", "Impossible"),
]


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


def sample_cases():
    for name, inp, exp in SAMPLES:
        gm = run(EXE_MAIN, inp)
        gb = run(EXE_BRUTE, inp)
        print("%s 回归: 期望 %s | 标程 %s | 暴力 %s" % (name, exp, gm, gb))
        if gm != exp or gb != exp:
            print("样例失败:", name)
            sys.exit(1)


def gen(rng):
    n = rng.randint(1, 12)
    c = rng.randint(0, 15)
    v = rng.choice([0, rng.randint(0, 30)])
    lines = ["%d %d %d" % (v, n, c)]
    for _ in range(n):
        kk = rng.choice([0, rng.randint(0, 20)])
        mc = rng.choice([0, rng.randint(0, 20)])
        lines.append("%d %d" % (kk, mc))
    return "\n".join(lines) + "\n"


def stress():
    rng = random.Random(20261018)
    for it in range(ROUNDS):
        inp = gen(rng)
        a = run(EXE_MAIN, inp)
        b = run(EXE_BRUTE, inp)
        if a != b:
            print("对拍不一致! 第 %d 组" % (it + 1))
            print("----- 输入 -----")
            print(inp, end="")
            print("----- 标程输出 -----", a)
            print("----- 暴力输出 -----", b)
            sys.exit(1)
    print("随机对拍: %d 组全部一致" % ROUNDS)


def boundary_cases():
    cases = [
        ("v=0 可直接填平", "0 1 5\n3 2\n", "5"),
        ("全部超体力", "10 2 1\n50 5\n10 2\n", "Impossible"),
        ("恰好装满且用尽体力", "100 2 10\n50 5\n50 5\n", "0"),
        ("零体力木石", "5 1 3\n5 0\n", "3"),
        ("零体积木石", "0 1 3\n0 4\n", "3"),
        ("总体积不足", "100 3 100\n10 1\n10 1\n10 1\n", "Impossible"),
        ("只需两块", "10 3 10\n5 1\n5 1\n5 1\n", "8"),
        ("超体积但体力不足", "100 1 1\n500 2\n", "Impossible"),
    ]
    for name, inp, exp in cases:
        gm = run(EXE_MAIN, inp)
        gb = run(EXE_BRUTE, inp)
        if gm != exp or gb != exp:
            print("边界失败: %s" % name)
            print("----- 输入 -----")
            print(inp, end="")
            print("期望 %s | 标程 %s | 暴力 %s" % (exp, gm, gb))
            sys.exit(1)
        print("边界通过: %-20s -> %s" % (name, gm))


def worst_case_timing():
    # n=1e4, c=1e4, 每块 (体积1, 体力1), v=1e4: 必须全搬 -> 用尽体力 -> 0
    n, c, v = 10000, 10000, 10000
    lines = ["%d %d %d" % (v, n, c)] + ["1 1"] * n
    inp = "\n".join(lines) + "\n"
    best = None
    out = None
    for _ in range(3):
        t0 = time.perf_counter()
        out = run(EXE_MAIN, inp)
        dt = time.perf_counter() - t0
        best = dt if best is None else min(best, dt)
    assert out == "0", "最坏用例答案异常: %s (期望 0)" % out
    print("最坏规模 (v=n=c=10000, 全 1 1): 输出 %s | 含进程启动最快 %.4f s" % (out, best))


def main():
    build()
    sample_cases()
    boundary_cases()
    stress()
    worst_case_timing()
    print("全部验证通过")


if __name__ == "__main__":
    main()
