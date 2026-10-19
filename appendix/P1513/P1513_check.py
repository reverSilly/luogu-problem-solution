#!/usr/bin/env python3
# P1513 绕钉子的长绳子 对拍脚本
#   1) 自动编译 标程 + 暴力
#   2) 官方样例 + 手造边界(单钉/两钉/三角形/矩形/R=0)
#   3) 300 组随机凸多边形(圆上取点, 逆时针)对拍
#   4) 最坏规模计时
# 任何一组不一致: 打印该组输入与双方输出, 并 sys.exit(1)
import math
import os
import random
import subprocess
import sys
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1513.cpp")
SRC_BRUTE = os.path.join(BASE, "P1513_brute.cpp")
EXE_MAIN = os.path.join(BASE, "P1513")
EXE_BRUTE = os.path.join(BASE, "P1513_brute")
CXX = ["g++", "-O2", "-std=c++17"]
ROUNDS = 300

# 官方样例(取自洛谷原题页 P1513 "输入输出样例 #1")
SAMPLE_IN = "4 1\n0.0 0.0\n2.0 0.0\n2.0 2.0\n0.0 2.0\n"
SAMPLE_OUT = "14.28"


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


def sample_and_boundary():
    cases = [
        ("官方样例", SAMPLE_IN, SAMPLE_OUT),
        ("单钉 N=1", "1 2.5\n0 0\n", "15.71"),          # 2π*2.5
        ("两钉 N=2", "2 1\n0 0\n10 0\n", "26.28"),       # 20+2π
        ("三角形", "3 1\n0 0\n4 0\n0 3\n", "18.28"),      # 12+2π
        ("矩形 R=2", "4 2\n-3 -1\n3 -1\n3 1\n-3 1\n", "28.57"),  # 16+4π
        ("R=0 退化为周长", "3 0\n0 0\n4 0\n0 3\n", "12.00"),
    ]
    for name, inp, exp in cases:
        gm = run(EXE_MAIN, inp)
        gb = run(EXE_BRUTE, inp)
        if gm != exp or gb != exp:
            print("用例失败: %s" % name)
            print("----- 输入 -----")
            print(inp, end="")
            print("期望 %s | 标程 %s | 暴力 %s" % (exp, gm, gb))
            sys.exit(1)
        print("用例通过: %-14s -> %s" % (name, gm))


def gen(rng):
    """圆上取点并按极角排序 => 严格凸多边形且逆时针。"""
    N = rng.randint(1, 10)
    R = round(rng.uniform(0.0, 5.0), 3)
    rr = rng.uniform(1.0, 90.0)
    angs = sorted(rng.uniform(0.0, 2 * math.pi) for _ in range(N))
    lines = ["%d %g" % (N, R)]
    for a in angs:
        lines.append("%.6f %.6f" % (rr * math.cos(a), rr * math.sin(a)))
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


def worst_case_timing():
    N = 100
    R = 1.0
    rr = 90.0
    rng = random.Random(1)
    angs = sorted(rng.uniform(0.0, 2 * math.pi) for _ in range(N))
    lines = ["%d %g" % (N, R)]
    for a in angs:
        lines.append("%.6f %.6f" % (rr * math.cos(a), rr * math.sin(a)))
    inp = "\n".join(lines) + "\n"
    best = None
    out = None
    for _ in range(5):
        t0 = time.perf_counter()
        out = run(EXE_MAIN, inp)
        dt = time.perf_counter() - t0
        best = dt if best is None else min(best, dt)
    print("最坏规模 (N=100 圆上取点): 输出 %s | 含进程启动最快 %.4f s" % (out, best))


def main():
    build()
    sample_and_boundary()
    stress()
    worst_case_timing()
    print("全部验证通过")


if __name__ == "__main__":
    main()
