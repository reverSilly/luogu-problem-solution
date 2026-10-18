#!/usr/bin/env python3
"""P1520 因式分解 对拍与验证脚本"""
import os
import random
import subprocess
import sys
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1520.cpp")
SRC_BRUTE = os.path.join(BASE, "P1520_brute.cpp")
EXE_MAIN = os.path.join(BASE, "P1520")
EXE_BRUTE = os.path.join(BASE, "P1520_brute")
CXX = ["g++", "-O2", "-std=c++17"]

SAMPLE_IN = "12\n"
SAMPLE_OUT = "(x-1)(x+1)(x^2+1)(x^2-x+1)(x^2+x+1)(x^4-x^2+1)"


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
    assert gm == SAMPLE_OUT and gb == SAMPLE_OUT, f"样例失败: gm={gm}, gb={gb}"
    print(f"官方样例通过: n=12 -> {gm}")


def boundary_cases():
    cases = [
        ("n=1 (唯一省略括号边界)", "1\n", "x-1"),
        ("n=2", "2\n", "(x-1)(x+1)"),
        ("n=3", "3\n", "(x-1)(x^2+x+1)"),
        ("n=4", "4\n", "(x-1)(x+1)(x^2+1)"),
        ("n=6 (同为2次: x^2-x+1 排在 x^2+x+1 前)", "6\n", "(x-1)(x+1)(x^2-x+1)(x^2+x+1)"),
        ("n=105 (首个出现绝对值>1系数的分圆多项式 -2x^7)", "105\n", None),
    ]
    for name, inp, exp in cases:
        gm = run(EXE_MAIN, inp)
        gb = run(EXE_BRUTE, inp)
        assert gm == gb, f"标程与暴力不一致 {name}: gm={gm}, gb={gb}"
        if exp is not None:
            assert gm == exp, f"边界失败 {name}: gm={gm}, exp={exp}"
        else:
            assert "-2x^7" in gm and "-2x^41" in gm
        print(f"边界通过: {name:<46s} -> {gm[:45]}{'...' if len(gm)>45 else ''}")


def stress():
    # 对拍 1..500 全量 + 501..5000 随机抽样 300 个(含高合成数 2310, 4620, 5000)
    test_ns = list(range(1, 501)) + [1155, 2310, 2520, 3465, 4620, 4999, 5000]
    rng = random.Random(20261016)
    test_ns += [rng.randint(501, 5000) for _ in range(300)]
    for n in test_ns:
        inp = f"{n}\n"
        gm = run(EXE_MAIN, inp)
        gb = run(EXE_BRUTE, inp)
        if gm != gb:
            print(f"对拍不一致! n={n}\n标程: {gm}\n暴力: {gb}")
            sys.exit(1)
    print(f"穷举+随机对拍: 共 {len(test_ns)} 个不同 n (含 1..500 全量与高合成数) 100% 一致")


def worst_case_timing():
    for n in (4620, 5000, 4999):
        best = None
        out = None
        for _ in range(5):
            t0 = time.perf_counter()
            out = run(EXE_MAIN, f"{n}\n")
            dt = time.perf_counter() - t0
            best = dt if best is None else min(best, dt)
        print(f"最坏规模 (n={n}): 输出长度 {len(out)} 字符, 含进程启动最快 {best:.4f} s")


def main():
    build()
    sample_case()
    boundary_cases()
    stress()
    worst_case_timing()
    print("全部验证通过")


if __name__ == "__main__":
    main()
