#!/usr/bin/env python3
"""P1525 [NOIP 2010 提高组] 关押罪犯 对拍与验证脚本"""
import os
import random
import subprocess
import sys
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1525.cpp")
SRC_BRUTE = os.path.join(BASE, "P1525_brute.cpp")
EXE_MAIN = os.path.join(BASE, "P1525")
EXE_BRUTE = os.path.join(BASE, "P1525_brute")
CXX = ["g++", "-O2", "-std=c++17"]
ROUNDS = 3000

SAMPLE_IN = """4 6
1 4 2534
2 3 3512
1 2 28351
1 3 6618
2 4 1805
3 4 12884
"""
SAMPLE_OUT = "3512"


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


def boundary_cases():
    cases = [
        ("二分图无冲突 (输出 0)", "4 4\n1 2 100\n2 3 200\n3 4 300\n1 4 400\n", "0"),
        ("单三角形奇环 (输出最小边)", "3 3\n1 2 10\n2 3 20\n1 3 30\n", "10"),
        ("无边图 M=0 (输出 0)", "5 0\n", "0"),
        ("多连通块各自含奇环", "6 6\n1 2 50\n2 3 60\n1 3 70\n4 5 80\n5 6 90\n4 6 100\n", "80"),
    ]
    for name, inp, exp in cases:
        gm = run(EXE_MAIN, inp)
        gb = run(EXE_BRUTE, inp)
        assert gm == exp and gb == exp, f"边界失败 {name}: gm={gm}, gb={gb}, exp={exp}"
        print(f"边界通过: {name:<36s} -> {gm}")


def gen(rng):
    n = rng.randint(2, 12)
    pairs = [(u, v) for u in range(1, n + 1) for v in range(u + 1, n + 1)]
    rng.shuffle(pairs)
    m = rng.randint(0, len(pairs))
    lines = [f"{n} {m}"]
    for u, v in pairs[:m]:
        w = rng.randint(1, 1000)
        lines.append(f"{u} {v} {w}")
    return "\n".join(lines) + "\n"


def stress():
    rng = random.Random(20261016)
    zero_cnt = 0
    pos_cnt = 0
    for it in range(ROUNDS):
        inp = gen(rng)
        gm = run(EXE_MAIN, inp)
        gb = run(EXE_BRUTE, inp)
        if gm != gb:
            print(f"对拍不一致! 第 {it+1} 组:\n{inp}标程: {gm} | 暴力: {gb}")
            sys.exit(1)
        if gm == "0":
            zero_cnt += 1
        else:
            pos_cnt += 1
    print(f"随机对拍: {ROUNDS} 组全部一致 (无冲突=0 共 {zero_cnt} 组, 有冲突>0 共 {pos_cnt} 组)")


def worst_case_timing():
    n = 20000
    m = 100000
    rng = random.Random(42)
    lines = [f"{n} {m}"]
    for i in range(m):
        u = (i % (n - 1)) + 1
        v = ((i * 7 + 3) % (n - u)) + u + 1
        w = rng.randint(1, 10**9)
        lines.append(f"{u} {v} {w}")
    inp = "\n".join(lines) + "\n"
    best = None
    out = None
    for _ in range(3):
        t0 = time.perf_counter()
        out = run(EXE_MAIN, inp)
        dt = time.perf_counter() - t0
        best = dt if best is None else min(best, dt)
    print(f"最坏规模 (N=20000, M=100000): 输出 {out}, 含进程启动最快 {best:.4f} s")


def main():
    build()
    sample_case()
    boundary_cases()
    stress()
    worst_case_timing()
    print("全部验证通过")


if __name__ == "__main__":
    main()
