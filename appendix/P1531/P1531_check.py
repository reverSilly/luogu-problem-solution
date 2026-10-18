#!/usr/bin/env python3
"""P1531 I Hate It 对拍与验证脚本"""
import os
import random
import subprocess
import sys
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1531.cpp")
SRC_BRUTE = os.path.join(BASE, "P1531_brute.cpp")
EXE_MAIN = os.path.join(BASE, "P1531")
EXE_BRUTE = os.path.join(BASE, "P1531_brute")
CXX = ["g++", "-O2", "-std=c++17"]
ROUNDS = 500

SAMPLE_IN = """5 6
1 2 3 4 5
Q 1 5
U 3 6
Q 3 4
Q 4 5
U 2 9
Q 1 5
"""
SAMPLE_OUT = "5\n6\n5\n9"


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
    print(f"官方样例通过 -> {gm.replace(chr(10), ' / ')}")


def gen(rng):
    n = rng.randint(1, 50)
    m = rng.randint(1, 80)
    lines = [f"{n} {m}", " ".join(str(rng.randint(1, 100)) for _ in range(n))]
    for _ in range(m):
        if rng.random() < 0.5:
            l = rng.randint(1, n)
            r = rng.randint(l, n)
            lines.append(f"Q {l} {r}")
        else:
            x = rng.randint(1, n)
            v = rng.randint(1, 150)  # 包含低于当前值(不修改)与高于当前值(修改)两种情况
            lines.append(f"U {x} {v}")
    return "\n".join(lines) + "\n"


def stress():
    rng = random.Random(20261016)
    for it in range(ROUNDS):
        inp = gen(rng)
        gm = run(EXE_MAIN, inp)
        gb = run(EXE_BRUTE, inp)
        if gm != gb:
            print(f"对拍不一致! 第 {it+1} 组:\n{inp}标程:\n{gm}\n暴力:\n{gb}")
            sys.exit(1)
    print(f"随机对拍: {ROUNDS} 组全部一致")


def worst_case_timing():
    n = m = 200000
    lines = [f"{n} {m}", " ".join(str((i * 17) % 1000 + 1) for i in range(n))]
    for i in range(m):
        if i & 1:
            l = (i % (n // 2)) + 1
            r = l + (n // 2) - 1
            lines.append(f"Q {l} {r}")
        else:
            lines.append(f"U {(i % n) + 1} {i + 1}")
    inp = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    out = run(EXE_MAIN, inp)
    dt = time.perf_counter() - t0
    print(f"最坏规模 (n=m=200000): 输出行数 {len(out.splitlines())}, 耗时 {dt:.4f} s")


def main():
    build()
    sample_case()
    stress()
    worst_case_timing()
    print("全部验证通过")


if __name__ == "__main__":
    main()
