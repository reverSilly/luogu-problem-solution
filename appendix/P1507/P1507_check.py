#!/usr/bin/env python3
# P1507 NASA的食物计划 对拍脚本
#   1) 自动编译 标程 + 暴力
#   2) 官方样例回归
#   3) 数千组随机数据对拍(含边界)
#   4) 最坏规模计时
# 任何一组不一致: 打印该组输入与双方输出, 并 sys.exit(1)
import os
import random
import subprocess
import sys
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1507.cpp")
SRC_BRUTE = os.path.join(BASE, "P1507_brute.cpp")
EXE_MAIN = os.path.join(BASE, "P1507")
EXE_BRUTE = os.path.join(BASE, "P1507_brute")
CXX = ["g++", "-O2", "-std=c++17"]
ROUNDS = 3000

# 官方样例(取自洛谷原题页 P1507 "输入输出样例 #1"; 本地 statement/P1507.md 未收录样例)
SAMPLE_IN = """320 350
4
160 40 120
80 110 240
220 70 310
40 400 220
"""
SAMPLE_OUT = "550"


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
    got_main = run(EXE_MAIN, SAMPLE_IN)
    got_brute = run(EXE_BRUTE, SAMPLE_IN)
    print("样例回归: 期望 %s | 标程 %s | 暴力 %s" % (SAMPLE_OUT, got_main, got_brute))
    if got_main != SAMPLE_OUT:
        print("样例失败(标程): 输入如下")
        print(SAMPLE_IN)
        sys.exit(1)
    if got_brute != SAMPLE_OUT:
        print("样例失败(暴力): 输入如下")
        print(SAMPLE_IN)
        sys.exit(1)


def gen(rng):
    """按题面输入格式生成小规模数据; 刻意覆盖 0 值/超容量/恰好装满等边界。"""
    H = rng.choice([0, 1, 2, rng.randint(1, 15)])
    T = rng.choice([0, 1, 2, rng.randint(1, 15)])
    n = rng.randint(0, 14)
    lines = ["%d %d" % (H, T), str(n)]
    for _ in range(n):
        # 体积/质量: 25% 恰好等于容量, 20% 差一点, 其余在 [0, 容量+3](可能超容量)
        r = rng.random()
        if r < 0.25:
            hi = H
        elif r < 0.45:
            hi = max(0, H - 1)
        else:
            hi = rng.randint(0, H + 3)
        r = rng.random()
        if r < 0.25:
            ti = T
        elif r < 0.45:
            ti = max(0, T - 1)
        else:
            ti = rng.randint(0, T + 3)
        # 卡路里: 10% 为 0, 30% 小, 其余到题面上界 500
        rr = rng.random()
        if rr < 0.10:
            ki = 0
        elif rr < 0.40:
            ki = rng.randint(0, 30)
        else:
            ki = rng.randint(0, 500)
        lines.append("%d %d %d" % (hi, ti, ki))
    return "\n".join(lines) + "\n"


def stress():
    rng = random.Random(20261016)
    for it in range(ROUNDS):
        inp = gen(rng)
        a = run(EXE_MAIN, inp)
        b = run(EXE_BRUTE, inp)
        if a != b:
            print("对拍不一致! 第 %d 组" % (it + 1))
            print("----- 输入 -----")
            print(inp, end="")
            print("----- 标程输出 -----")
            print(a)
            print("----- 暴力输出 -----")
            print(b)
            sys.exit(1)
    print("随机对拍: %d 组全部一致" % ROUNDS)


def worst_case_timing():
    # 真的最坏: H=T=400, n=50, 每件 (1,1,*) => 内层两重循环每次都跑满 400*400
    H = T = 400
    n = 50
    lines = ["%d %d" % (H, T), str(n)] + ["1 1 500"] * n
    inp = "\n".join(lines) + "\n"
    best = None
    for _ in range(5):
        t0 = time.perf_counter()
        out = run(EXE_MAIN, inp)
        dt = time.perf_counter() - t0
        best = dt if best is None else min(best, dt)
    # 真最坏答案应恰为 50*500=25000(体积 50<=400, 质量 50<=400, 全部可选)
    assert out == "25000", "最坏用例答案异常: %s" % out
    print("最坏规模 (H=T=400,n=50,全 1 1 500): 输出 %s, 含进程启动最快一次 %.4f s" % (out, best))
    return best


def boundary_cases():
    cases = [
        ("一件都放不下", "5 5\n1\n6 6 100\n", "0"),
        ("n=0", "10 10\n0\n", "0"),
        ("恰好装满", "10 10\n1\n10 10 7\n", "7"),
        ("装不满也只取一件", "10 10\n1\n5 5 3\n", "3"),
        ("零体积零质量", "0 0\n2\n0 0 5\n0 0 9\n", "14"),
        ("贪心反例(选 B+C 优于 A)", "10 10\n3\n5 5 9\n4 6 8\n6 4 8\n", "16"),
        ("只做体积维会错(真值100)", "10 5\n4\n3 5 100\n3 1 1\n3 1 1\n3 1 1\n", "100"),
        ("合并二维会错(真值10)", "2 5\n2\n2 3 10\n1 1 1\n", "10"),
        ("超容量件被忽略", "3 3\n2\n400 400 999\n1 1 5\n", "5"),
    ]
    for name, inp, exp in cases:
        gm = run(EXE_MAIN, inp)
        gb = run(EXE_BRUTE, inp)
        if gm != exp or gb != exp:
            print("边界用例失败: %s" % name)
            print("----- 输入 -----")
            print(inp, end="")
            print("期望 %s | 标程 %s | 暴力 %s" % (exp, gm, gb))
            sys.exit(1)
        print("边界通过: %-26s -> %s" % (name, gm))


def main():
    build()
    sample_case()
    boundary_cases()
    stress()
    worst_case_timing()
    print("全部验证通过")


if __name__ == "__main__":
    main()
