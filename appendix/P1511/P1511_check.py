#!/usr/bin/env python3
"""P1511 [SDOI2006] 数字串位置 / 超长数字串 对拍与验证脚本"""
import os
import random
import subprocess
import sys
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1511.cpp")
SRC_BRUTE = os.path.join(BASE, "P1511_brute.cpp")
EXE_MAIN = os.path.join(BASE, "P1511")
EXE_BRUTE = os.path.join(BASE, "P1511_brute")
CXX = ["g++", "-O2", "-std=c++17"]
ROUNDS = 3000


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
        ("单字符 1", "1\n", "1"),
        ("单字符 9", "9\n", "9"),
        ("单字符 0 (落在10)", "0\n", "11"),
        ("跨数拼接 910", "910\n", "9"),
        ("全零串 000", "000\n", str(1 * 9 + 2 * 90 + 3 * 900 + 2)),  # 1000 的第2位(0-based p=1): 2890+1=2891
        ("全9进位 99100", "99100\n", str(1 * 9 + 2 * 89 + 1)),       # 99 起始位置: 9 + 178 + 1 = 188
        ("r<p 且 P=10^(r-1) 边界 (9910)", "9910\n", "188"),
        ("r<p 且低位全9且P=10 (X=1009->1010)", "09101\n", str(1 * 9 + 2 * 90 + 3 * 900 + (1009 - 1000) * 4 + 2 + 1)),
        ("跨数错位拼接 07388 (38807+38808)", "07388\n", "182928"),
    ]
    for name, inp, exp in cases:
        gm = run(EXE_MAIN, inp)
        gb = run(EXE_BRUTE, inp)
        if gm != exp or gb != exp:
            print(f"边界失败 [{name}]: 期望 {exp} | 标程 {gm} | 暴力 {gb}")
            sys.exit(1)
        print(f"边界通过: {name:<42s} -> {gm}")


def stress():
    S = "".join(str(i) for i in range(1, 200000))
    rng = random.Random(20261016)
    for it in range(ROUNDS):
        if it < 1500:
            # 直接生成任意 1..4 位数字串(含前导 0、全 0、连续 9, 必在 X<=10000 内出现)
            length = rng.randint(1, 4)
            alphabet = "0123456789" if rng.random() < 0.7 else "019"
            A = "".join(rng.choice(alphabet) for _ in range(length))
        else:
            # 从 S 中截取子串 (长度 1..15)
            length = rng.randint(1, 15)
            pos = rng.randint(0, 400000)
            A = S[pos : pos + length]
        exp = str(S.find(A) + 1)
        got = run(EXE_MAIN, A + "\n")
        if got != exp:
            print(f"对拍不一致! 第 {it+1} 组 A={A}")
            print(f"标程: {got} | 暴力: {exp}")
            sys.exit(1)
    # 额外抽检 50 组กับ C++ 暴力程序可执行文件的端到端对拍
    for it in range(50):
        length = rng.randint(1, 5)
        A = "".join(rng.choice("0123456789") for _ in range(length))
        gm = run(EXE_MAIN, A + "\n")
        gb = run(EXE_BRUTE, A + "\n")
        if gm != gb:
            print(f"C++暴力端到端不一致! A={A}: 标程 {gm} | 暴力 {gb}")
            sys.exit(1)
    print(f"随机与子串对拍: {ROUNDS} 组 + 50 组 C++ 端到端全部一致")


def worst_case_timing():
    A_zeros = "0" * 200
    best = None
    out = None
    for _ in range(5):
        t0 = time.perf_counter()
        out = run(EXE_MAIN, A_zeros + "\n")
        dt = time.perf_counter() - t0
        best = dt if best is None else min(best, dt)
    exp_val = 201 * (10**200) - (10**201 - 1) // 9 + 2
    assert out == str(exp_val), f"全0最坏用例答案错误: {out} != {exp_val}"
    print(f"最坏规模 (|A|=200 全0): 输出位数 {len(out)}, 含进程启动最快 {best:.4f} s")


def main():
    build()
    boundary_cases()
    stress()
    worst_case_timing()
    print("全部验证通过")


if __name__ == "__main__":
    main()
