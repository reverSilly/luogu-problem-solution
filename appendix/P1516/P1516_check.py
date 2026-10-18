#!/usr/bin/env python3
"""P1516 青蛙的约会 对拍与验证脚本"""
import os
import random
import subprocess
import sys
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1516.cpp")
SRC_BRUTE = os.path.join(BASE, "P1516_brute.cpp")
EXE_MAIN = os.path.join(BASE, "P1516")
EXE_BRUTE = os.path.join(BASE, "P1516_brute")
CXX = ["g++", "-O2", "-std=c++17"]
ROUNDS = 4000

SAMPLE_IN = "1 2 3 4 5\n"
SAMPLE_OUT = "4"


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
    print(f"官方样例通过: {SAMPLE_IN.strip()} -> {gm}")


def boundary_cases():
    cases = [
        ("m == n 且 x != y mod L (相对静止必无解)", "1 2 5 5 10\n", "Impossible"),
        ("m == n 但 x === y (mod L) (0步即同点)", "1 11 5 5 10\n", "0"),
        ("m < n (负系数归一测试)", "10 2 1 3 12\n", "4"),
        ("gcd(m-n, L) > 1 无解 (奇偶步差不整除)", "1 2 2 4 6\n", "Impossible"),
        ("gcd(m-n, L) > 1 有解 (模 L/g 缩减周期)", "1 5 2 4 6\n", "1"),
        ("上界大数溢出测试 (L=2.1e9, 乘积超 i64)", "1 2000000000 1999999999 1 2099999999\n", None),
    ]
    for name, inp, exp in cases:
        gm = run(EXE_MAIN, inp)
        if exp is not None:
            gb = run(EXE_BRUTE, inp)
            assert gm == exp and gb == exp, f"边界失败 {name}: gm={gm}, gb={gb}, exp={exp}"
        else:
            # 用 Python 大整数验算大数用例
            x, y, m, n, L = map(int, inp.split())
            t = int(gm)
            assert (x + m * t - (y + n * t)) % L == 0 and 0 <= t < L
        print(f"边界通过: {name:<44s} -> {gm}")


def stress():
    rng = random.Random(20261016)
    imp_cnt = 0
    ok_cnt = 0
    for it in range(ROUNDS):
        L = rng.randint(2, 600)
        x = rng.randint(1, 1000)
        y = rng.randint(1, 1000)
        while x == y:
            y = rng.randint(1, 1000)
        m = rng.randint(1, 1000)
        n = rng.randint(1, 1000)
        inp = f"{x} {y} {m} {n} {L}\n"
        gm = run(EXE_MAIN, inp)
        gb = run(EXE_BRUTE, inp)
        if gm != gb:
            print(f"对拍不一致! 第 {it+1} 组: {inp.strip()} | 标程={gm} | 暴力={gb}")
            sys.exit(1)
        if gm == "Impossible":
            imp_cnt += 1
        else:
            ok_cnt += 1
    print(f"随机对拍: {ROUNDS} 组全部一致 (有解 {ok_cnt} 组, Impossible {imp_cnt} 组)")


def worst_case_timing():
    inp = "1 2000000000 1999999999 1 2099999999\n"
    best = None
    out = None
    for _ in range(5):
        t0 = time.perf_counter()
        out = run(EXE_MAIN, inp)
        dt = time.perf_counter() - t0
        best = dt if best is None else min(best, dt)
    print(f"最坏规模 (L=2.1e9): 输出 {out}, 含进程启动最快 {best:.4f} s")


def main():
    build()
    sample_case()
    boundary_cases()
    stress()
    worst_case_timing()
    print("全部验证通过")


if __name__ == "__main__":
    main()
