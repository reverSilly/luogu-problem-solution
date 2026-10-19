#!/usr/bin/env python3
# P1508 Likecloud-吃、吃、吃 对拍脚本
#   1) 自动编译 标程 + 暴力
#   2) 官方样例回归
#   3) 400 组随机小规模对拍(含极端值)
#   4) 最坏规模计时 + 大范围可判定用例
# 任何一组不一致: 打印该组输入与双方输出, 并 sys.exit(1)
import os
import random
import subprocess
import sys
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1508.cpp")
SRC_BRUTE = os.path.join(BASE, "P1508_brute.cpp")
EXE_MAIN = os.path.join(BASE, "P1508")
EXE_BRUTE = os.path.join(BASE, "P1508_brute")
CXX = ["g++", "-O2", "-std=c++17"]
ROUNDS = 400

# 官方样例(取自洛谷原题页 P1508 "输入输出样例 #1"; 本地 statement/P1508.md 未收录样例)
SAMPLE_IN = """6 7
16 4 3 12 6 0 3
4 -5 6 7 0 0 2
6 0 -1 -2 3 6 8
5 3 4 0 0 -2 7
-1 7 4 0 7 -5 6
0 -1 3 4 12 4 2
"""
SAMPLE_OUT = "41"


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
    """按题面输入格式生成小规模数据; n 必须为奇数。"""
    m = rng.randint(1, 8)
    n = rng.choice([1, 3, 5, 7, 9])
    low, high = -20, 20
    if rng.random() < 0.3:
        low, high = -100000, 100000
    rows = []
    for _ in range(m):
        rows.append(" ".join(str(rng.randint(low, high)) for _ in range(n)))
    return "%d %d\n" % (m, n) + "\n".join(rows) + "\n"


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
    m, n = 200, 199
    # 1) 全 1 大矩阵: 每行必吃一个, 答案恒为 m
    inp1 = "%d %d\n" % (m, n) + "\n".join(" ".join("1" for _ in range(n)) for _ in range(m)) + "\n"
    out1 = run(EXE_MAIN, inp1)
    assert out1 == str(m), "全1最坏用例答案异常: %s (期望 %d)" % (out1, m)
    # 2) 随机 +-1e5 大矩阵计时
    rng = random.Random(20261018)
    rows = [" ".join(str(rng.randint(-100000, 100000)) for _ in range(n)) for _ in range(m)]
    inp2 = "%d %d\n" % (m, n) + "\n".join(rows) + "\n"
    best = None
    out2 = None
    for _ in range(5):
        t0 = time.perf_counter()
        out2 = run(EXE_MAIN, inp2)
        dt = time.perf_counter() - t0
        best = dt if best is None else min(best, dt)
    print("最坏规模 (m=200,n=199): 全1 -> %s | 随机 -> %s | 含进程启动最快 %.4f s"
          % (out1, out2, best))


def main():
    build()
    sample_case()
    stress()
    worst_case_timing()
    print("全部验证通过")


if __name__ == "__main__":
    main()
