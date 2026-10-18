#!/usr/bin/env python3
"""P1524 十字绣 对拍与验证脚本"""
import os
import random
import subprocess
import sys
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1524.cpp")
SRC_BRUTE = os.path.join(BASE, "P1524_brute.cpp")
EXE_MAIN = os.path.join(BASE, "P1524")
EXE_BRUTE = os.path.join(BASE, "P1524_brute")
CXX = ["g++", "-O2", "-std=c++17"]
ROUNDS = 2000


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


def boundary_cases():
    cases = [
        ("全空画布 (0针)", "2 2\n..\n..\n..\n..\n", "0"),
        ("单根正面线 (1针)", "1 1\n/\n.\n", "1"),
        ("同一格正反重合重边 (欧拉闭回路=1针)", "1 1\n/\n/\n", "1"),
        ("同一点两正两反 (差为0闭回路=1针)", "1 1\nX\n.\n", "2"),
        ("两个独立连通分量", "1 2\n/\\\n..\n", "2"),
    ]
    for name, inp, exp in cases:
        gm = run(EXE_MAIN, inp)
        gb = run(EXE_BRUTE, inp)
        assert gm == exp and gb == exp, f"边界失败 {name}: gm={gm}, gb={gb}, exp={exp}"
        print(f"边界通过: {name:<38s} -> {gm}")


def gen(rng):
    n = rng.randint(1, 12)
    m = rng.randint(1, 12)
    chars = "..../\\X"
    lines = [f"{n} {m}"]
    for _ in range(2 * n):
        lines.append("".join(rng.choice(chars) for _ in range(m)))
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


def worst_case_timing():
    n = m = 200
    lines = [f"{n} {m}"] + ["X" * m for _ in range(2 * n)]
    inp = "\n".join(lines) + "\n"
    best = None
    out = None
    for _ in range(5):
        t0 = time.perf_counter()
        out = run(EXE_MAIN, inp)
        dt = time.perf_counter() - t0
        best = dt if best is None else min(best, dt)
    print(f"最坏规模 (N=M=200 全X): 输出 {out}, 含进程启动最快 {best:.4f} s")


def main():
    build()
    boundary_cases()
    stress()
    worst_case_timing()
    print("全部验证通过")


if __name__ == "__main__":
    main()
