#!/usr/bin/env python3
"""P1528 切蛋糕 对拍与验证脚本"""
import os
import random
import subprocess
import sys
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1528.cpp")
SRC_BRUTE = os.path.join(BASE, "P1528_brute.cpp")
EXE_MAIN = os.path.join(BASE, "P1528")
EXE_BRUTE = os.path.join(BASE, "P1528_brute")
CXX = ["g++", "-O2", "-std=c++17"]
ROUNDS = 1000

SAMPLE_IN = """4
30
40
50
25
10
15
16
17
18
19
20
21
25
24
30
"""
SAMPLE_OUT = "7"


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
    print(f"官方样例通过 -> {gm}")


def gen(rng):
    n = rng.randint(1, 5)
    m = rng.randint(1, 9)
    cakes = [rng.randint(1, 40) for _ in range(n)]
    mouths = [rng.randint(1, 25) for _ in range(m)]
    lines = [str(n)] + list(map(str, cakes)) + [str(m)] + list(map(str, mouths))
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


def main():
    build()
    sample_case()
    stress()
    print("全部验证通过")


if __name__ == "__main__":
    main()
