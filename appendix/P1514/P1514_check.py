#!/usr/bin/env python3
"""P1514 [NOIP 2010 提高组] 引水入城 对拍与验证脚本"""
import os
import random
import subprocess
import sys
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1514.cpp")
SRC_BRUTE = os.path.join(BASE, "P1514_brute.cpp")
EXE_MAIN = os.path.join(BASE, "P1514")
EXE_BRUTE = os.path.join(BASE, "P1514_brute")
CXX = ["g++", "-O2", "-std=c++17"]
ROUNDS = 3000

SAMPLE_1_IN = """2 5
9 1 5 4 3
8 7 6 1 2
"""
SAMPLE_1_OUT = "1\n1"

SAMPLE_2_IN = """3 6
8 4 5 6 4 4
7 3 4 3 3 3
3 2 2 1 1 2
"""
SAMPLE_2_OUT = "1\n3"


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


def sample_cases():
    for idx, (sin, sout) in enumerate(((SAMPLE_1_IN, SAMPLE_1_OUT), (SAMPLE_2_IN, SAMPLE_2_OUT)), 1):
        gm = run(EXE_MAIN, sin)
        gb = run(EXE_BRUTE, sin)
        assert gm == sout and gb == sout, f"样例 {idx} 失败: gm={gm}, gb={gb}, exp={sout}"
        print(f"官方样例 #{idx} 通过 -> {gm.replace(chr(10), ' / ')}")


def gen(rng):
    n = rng.randint(1, 10)
    m = rng.randint(1, 12)
    lines = [f"{n} {m}"]
    # 70% 生成整体北高南低的地形(更高概率全可达, 重点压测连续区间引理与贪心覆盖)
    bias = rng.random() < 0.7
    for r in range(n):
        row = []
        for c in range(m):
            if bias:
                base = (n - r) * 15
                row.append(str(max(1, base + rng.randint(-12, 12))))
            else:
                row.append(str(rng.randint(1, 30)))
        lines.append(" ".join(row))
    return "\n".join(lines) + "\n"


def stress():
    rng = random.Random(20261016)
    ok_cnt = 0
    fail_cnt = 0
    for it in range(ROUNDS):
        inp = gen(rng)
        gm = run(EXE_MAIN, inp)
        gb = run(EXE_BRUTE, inp)
        if gm != gb:
            print(f"对拍不一致! 第 {it+1} 组:\n{inp}标程:\n{gm}\n暴力:\n{gb}")
            sys.exit(1)
        if gm.startswith("1"):
            ok_cnt += 1
        else:
            fail_cnt += 1
    print(f"随机对拍: {ROUNDS} 组全部一致 (其中全可达 {ok_cnt} 组, 不可全达 {fail_cnt} 组)")


def boundary_cases():
    cases = [
        ("单行 N=1 (Test #4 模型: 平台不流动)", "1 5\n5 5 5 5 5\n", "1\n5"),
        ("单行 N=1 严格递减 (1座可覆盖全行)", "1 5\n9 8 7 6 5\n", "1\n1"),
        ("底行横向流动才能覆盖", "2 3\n9 1 1\n8 7 6\n", "1\n1"),
        ("全部不可达(北低南高)", "2 4\n1 1 1 1\n9 9 9 9\n", "0\n4"),
        ("单点 1x1", "1 1\n42\n", "1\n1"),
    ]
    for name, inp, exp in cases:
        gm = run(EXE_MAIN, inp)
        gb = run(EXE_BRUTE, inp)
        assert gm == exp and gb == exp, f"边界失败 {name}: gm={gm}, gb={gb}, exp={exp}"
        print(f"边界通过: {name:<36s} -> {gm.replace(chr(10), ' / ')}")


def worst_case_timing():
    n = m = 500
    lines = [f"{n} {m}"]
    for r in range(n):
        base = (n - r) * 1000
        lines.append(" ".join(str(base + (c % 7)) for c in range(m)))
    inp = "\n".join(lines) + "\n"
    best = None
    out = None
    for _ in range(3):
        t0 = time.perf_counter()
        out = run(EXE_MAIN, inp)
        dt = time.perf_counter() - t0
        best = dt if best is None else min(best, dt)
    print(f"最坏规模 (N=M=500): 输出 {out.replace(chr(10), ' / ')}, 含进程启动最快 {best:.4f} s")


def main():
    build()
    sample_cases()
    boundary_cases()
    stress()
    worst_case_timing()
    print("全部验证通过")


if __name__ == "__main__":
    main()
