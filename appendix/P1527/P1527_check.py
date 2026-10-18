#!/usr/bin/env python3
"""P1527 [国家集训队] 矩阵乘法 对拍与验证脚本"""
import os
import random
import subprocess
import sys
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1527.cpp")
SRC_BRUTE = os.path.join(BASE, "P1527_brute.cpp")
EXE_MAIN = os.path.join(BASE, "P1527")
EXE_BRUTE = os.path.join(BASE, "P1527_brute")
CXX = ["g++", "-O2", "-std=c++17"]
ROUNDS = 500

SAMPLE_IN = """2 2
2 1
3 4
1 2 1 2 1
1 1 2 2 3
"""
SAMPLE_OUT = "1\n3"


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
    n = rng.randint(1, 15)
    q = rng.randint(1, 30)
    lines = [f"{n} {q}"]
    # 包含重复权值、0、10^9 边界
    for _ in range(n):
        row = [str(rng.choice([0, 1, 5, 10**9, rng.randint(0, 50)])) for _ in range(n)]
        lines.append(" ".join(row))
    for _ in range(q):
        x1 = rng.randint(1, n)
        x2 = rng.randint(x1, n)
        y1 = rng.randint(1, n)
        y2 = rng.randint(y1, n)
        area = (x2 - x1 + 1) * (y2 - y1 + 1)
        k = rng.randint(1, area)
        lines.append(f"{x1} {y1} {x2} {y2} {k}")
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
    print(f"随机对拍: {ROUNDS} 组矩阵(共约 7500 次子矩形第 k 小询问)全部一致")


def worst_case_timing():
    rng = random.Random(77)
    n = 500
    q = 60000
    lines = [f"{n} {q}"]
    for i in range(n):
        lines.append(" ".join(str((i * 503 + j * 997) % 1000000) for j in range(n)))
    for i in range(q):
        x1 = (i % 250) + 1
        y1 = ((i * 7) % 250) + 1
        x2 = x1 + 200
        y2 = y1 + 200
        k = (i * 13) % (201 * 201) + 1
        lines.append(f"{x1} {y1} {x2} {y2} {k}")
    inp = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    out = run(EXE_MAIN, inp)
    dt = time.perf_counter() - t0
    print(f"最坏规模 (n=500, q=60000): 输出行数 {len(out.splitlines())}, 耗时 {dt:.4f} s")


def main():
    build()
    sample_case()
    stress()
    worst_case_timing()
    print("全部验证通过")


if __name__ == "__main__":
    main()
