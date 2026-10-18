#!/usr/bin/env python3
"""P1533 可怜的狗狗 对拍与验证脚本"""
import os
import random
import subprocess
import sys
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1533.cpp")
SRC_BRUTE = os.path.join(BASE, "P1533_brute.cpp")
EXE_MAIN = os.path.join(BASE, "P1533")
EXE_BRUTE = os.path.join(BASE, "P1533_brute")
CXX = ["g++", "-O2", "-std=c++17"]
ROUNDS = 1000

SAMPLE_IN = """7 2
1 5 2 6 3 7 4
1 5 3
2 7 1
"""
SAMPLE_OUT = "3\n2"


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
    n = rng.randint(2, 40)
    vals = rng.sample(range(0, 10000), n)
    # 生成互不包含的区间族: 左端点严格递增、右端点严格递增, 再打乱询问顺序
    m = rng.randint(1, min(20, n))
    ls = sorted(rng.sample(range(1, n + 1), m))
    rs = sorted(rng.sample(range(1, n + 1), m))
    qs = []
    for l, r in zip(ls, rs):
        if l > r:
            l, r = r, l
        k = rng.randint(1, r - l + 1)
        qs.append((l, r, k))
    rng.shuffle(qs)
    lines = [f"{n} {m}", " ".join(map(str, vals))] + [f"{l} {r} {k}" for l, r, k in qs]
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
    n = 300000
    m = 50000
    lines = [f"{n} {m}", " ".join(str((i * 1000003) % (2**31 - 1)) for i in range(n))]
    for i in range(m):
        l = i + 1
        r = i + (n - m)
        k = (i % (n - m)) + 1
        lines.append(f"{l} {r} {k}")
    inp = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    out = run(EXE_MAIN, inp)
    dt = time.perf_counter() - t0
    print(f"最坏规模 (n=300000, m=50000): 输出行数 {len(out.splitlines())}, 耗时 {dt:.4f} s")


def main():
    build()
    sample_case()
    stress()
    worst_case_timing()
    print("全部验证通过")


if __name__ == "__main__":
    main()
