#!/usr/bin/env python3
"""P1523 旅行商简化版 对拍与验证脚本"""
import os
import random
import subprocess
import sys
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1523.cpp")
SRC_BRUTE = os.path.join(BASE, "P1523_brute.cpp")
EXE_MAIN = os.path.join(BASE, "P1523")
EXE_BRUTE = os.path.join(BASE, "P1523_brute")
CXX = ["g++", "-O2", "-std=c++17"]
ROUNDS = 2000

SAMPLE_IN = """7
0 6
1 0
2 3
5 4
6 1
7 5
8 2
"""
SAMPLE_OUT = "25.58"


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
    n = rng.randint(2, 14)
    # 保证最西端和最东端唯一, 中间点刻意打乱顺序并允许部分相同 x 坐标
    xs = [0] + [rng.randint(1, 50) for _ in range(n - 2)] + [51]
    pts = set()
    pts.add((0, rng.randint(-50, 50)))
    pts.add((51, rng.randint(-50, 50)))
    while len(pts) < n:
        pts.add((rng.randint(1, 50), rng.randint(-50, 50)))
    pts = list(pts)
    rng.shuffle(pts)  # 刻意乱序输入, 验证排序必要性
    lines = [str(n)] + [f"{x} {y}" for x, y in pts]
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
    print(f"随机乱序对拍: {ROUNDS} 组全部一致")


def boundary_cases():
    cases = [
        ("两点回路 (n=2, 3-4-5三角形往返=10)", "2\n0 0\n3 4\n", "10.00"),
        ("乱序输入官方样例", "7\n6 1\n0 6\n8 2\n2 3\n1 0\n7 5\n5 4\n", "25.58"),
        ("极端 32 位坐标防 int 减法溢出 (-2e9 到 +2e9)", "2\n-2000000000 0\n2000000000 0\n", "8000000000.00"),
    ]
    for name, inp, exp in cases:
        gm = run(EXE_MAIN, inp)
        gb = run(EXE_BRUTE, inp)
        assert gm == exp and gb == exp, f"边界失败 {name}: gm={gm}, gb={gb}, exp={exp}"
        print(f"边界通过: {name:<44s} -> {gm}")


def worst_case_timing():
    n = 1000
    lines = [str(n)] + [f"{i * 100} {(i * 137) % 1000}" for i in range(n)]
    inp = "\n".join(lines) + "\n"
    best = None
    out = None
    for _ in range(5):
        t0 = time.perf_counter()
        out = run(EXE_MAIN, inp)
        dt = time.perf_counter() - t0
        best = dt if best is None else min(best, dt)
    print(f"最坏规模 (n=1000): 输出 {out}, 含进程启动最快 {best:.4f} s")


def main():
    build()
    sample_case()
    boundary_cases()
    stress()
    worst_case_timing()
    print("全部验证通过")


if __name__ == "__main__":
    main()
