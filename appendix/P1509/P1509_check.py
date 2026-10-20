#!/usr/bin/env python3
# P1509 找啊找啊找GF 对拍脚本
#   1) 自动编译 标程 + 暴力
#   2) 官方样例回归
#   3) 400 组随机小规模对拍(含边界)
#   4) 最坏规模计时 + 可判定大用例
# 任何一组不一致: 打印该组输入与双方输出, 并 sys.exit(1)
import os
import random
import subprocess
import sys
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1509.cpp")
SRC_BRUTE = os.path.join(BASE, "P1509_brute.cpp")
EXE_MAIN = os.path.join(BASE, "P1509")
EXE_BRUTE = os.path.join(BASE, "P1509_brute")
CXX = ["g++", "-O2", "-std=c++17"]
ROUNDS = 400

# 官方样例(取自洛谷原题页 P1509 "输入输出样例 #1")
SAMPLE_IN = """4
1 2 5
2 1 6
2 2 2
2 2 3
5 5
"""
SAMPLE_OUT = "13"


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
    print("样例回归: 期望 %s | 标程 %s | 暴力 %s" % (SAMPLE_OUT, gm, gb))
    if gm != SAMPLE_OUT or gb != SAMPLE_OUT:
        print("样例失败")
        sys.exit(1)


def gen(rng):
    n = rng.randint(1, 12)
    m = rng.randint(1, 12)
    r = rng.randint(1, 12)
    lines = [str(n)]
    for _ in range(n):
        a = rng.randint(1, max(1, m + 2))
        b = rng.randint(1, max(1, r + 2))
        t = rng.randint(1, 1000)
        lines.append("%d %d %d" % (a, b, t))
    lines.append("%d %d" % (m, r))
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
        ("一个都泡不到", "2\n100 100 5\n100 100 5\n5 5\n", "0"),
        ("恰好装下4个", "4\n1 1 1\n1 1 1\n1 1 1\n1 1 1\n4 4\n", "4"),
        ("人数优先于时间", "2\n1 1 1000\n1 1 1\n2 2\n", "1001"),  # 两个都要(人数2), 时间1001
        ("只能泡一个(时间大)", "3\n5 5 999\n6 1 1\n1 6 1\n5 5\n", "999"),  # 只有第1个装得下
        ("两个单独可行取时间小", "2\n3 1 5\n1 1 7\n3 3\n", "5"),  # 两个单独都装得下(人数1), 取时间小的5
        ("预算/点数各差一点", "2\n3 3 8\n3 3 9\n3 2\n", "0"),  # rp 需<=2, 两个rp3超, 泡不到
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
        print("边界通过: %-18s -> %s" % (name, gm))


def worst_case_timing():
    # n=100, m=r=100, 全 (1,1,1000): 人数上限 100, 时间 100*1000
    lines = ["100"] + ["1 1 1000"] * 100 + ["100 100"]
    inp = "\n".join(lines) + "\n"
    best = None
    out = None
    for _ in range(5):
        t0 = time.perf_counter()
        out = run(EXE_MAIN, inp)
        dt = time.perf_counter() - t0
        best = dt if best is None else min(best, dt)
    assert out == "100000", "最坏用例答案异常: %s (期望 100000)" % out
    print("最坏规模 (n=100,m=r=100,全 1 1 1000): 输出 %s | 含进程启动最快 %.4f s" % (out, best))


def main():
    build()
    sample_case()
    boundary_cases()
    stress()
    worst_case_timing()
    print("全部验证通过")


if __name__ == "__main__":
    main()
