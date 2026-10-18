#!/usr/bin/env python3
"""P1526 [NOI2003] 智破连环阵 对拍与验证脚本"""
import os
import random
import subprocess
import sys
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1526.cpp")
SRC_BRUTE = os.path.join(BASE, "P1526_brute.cpp")
EXE_MAIN = os.path.join(BASE, "P1526")
EXE_BRUTE = os.path.join(BASE, "P1526_brute")
CXX = ["g++", "-O2", "-std=c++17"]
ROUNDS = 1000


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
    # 关键反例: 贪心选当前最远炸弹 B0(覆盖 0..2) 会用掉后面唯一能覆盖 4 的炸弹 B0!
    # 武器 0..4:
    # B0 覆盖 {0,1,2, 4} (在 0 处可推进到 2, 在 4 处也是唯一能炸 4 的)
    # B1 覆盖 {0,1}
    # B2 覆盖 {2,3}
    # 若在 0 处贪心选 B0(推进到 2), 下一步用 B2(推进到 3), 到武器 4 时无炸弹可用!
    # 正确顺序: B1(0..1) -> B2(2..3) -> B0(4) 共 3 枚!
    trap = """5 3 10
0 0
0 5
0 20
0 25
100 0
50 0
0 2
0 22
"""
    # 检查距离: k=10? 不, B0 需要同时离 (0,0),(0,5),(0,20) 和 (100,0) 近 -> 调整坐标让 B0 处在圆心
    # 设 k=50:
    # W0=(0,0), W1=(0,10), W2=(30,0), W3=(30,40), W4=(-30,0)
    # B0=(0,0) 半径 50: 到 W0(0), W1(10), W2(30), W3(50), W4(30) 全部 <=50! (如果让 W3=(30,45), d^2=900+2025=2925>2500 不覆盖 W3!)
    # 于是 B0=(0,0) 覆盖 {W0, W1, W2, W4}, 不覆盖 W3!
    # B1=(0,20): 到 W0(20<=50), W1(10<=50), 到 W2=(30,0) 为 1300<=2500 -> 把 W2 放到 (40,0):
    # 重新精确设置 k=10 (k^2=100):
    # W0=(0,0), W1=(6,0), W2=(8,0), W3=(8,15), W4=(-8,0)
    # B0=(0,0): 到 W0(0), W1(36), W2(64), W3(64+225=289>100 否), W4(64<=100 是) => B0 覆盖 {0,1,2, 4}!
    # B1=(3,9): 到 W0(9+81=90<=100), W1(9+81=90<=100), W2(25+81=106>100 否), W3(25+36=61<=100), W4(121+81>100 否) => B1 覆盖 {0,1, 3}
    # B2=(8,10): 到 W0(164>100), W1(4+100=104>100), W2(100<=100), W3(25<=100), W4(256+100>100) => B2 覆盖 {2,3}!
    trap_exact = """5 3 10
0 0
6 0
8 0
8 15
-8 0
0 0
3 9
8 10
"""
    gm = run(EXE_MAIN, trap_exact)
    gb = run(EXE_BRUTE, trap_exact)
    assert gm == "3" and gb == "3", f"贪心陷阱反例失败: gm={gm}, gb={gb}"
    print(f"边界通过: 贪心锁死反例 (B1->B2->B0 = 3枚) -> {gm}")


def gen(rng):
    m = rng.randint(1, 12)
    n = rng.randint(max(2, m // 2), 10)
    k = rng.randint(25, 60)
    # 先生成一条保证有解的基准链
    wx = [rng.randint(0, 100) for _ in range(m)]
    wy = [rng.randint(0, 100) for _ in range(m)]
    bx = [wx[i % m] for i in range(n)]
    by = [wy[i % m] for i in range(n)]
    # 保证每个武器至少被一个专属炸弹覆盖
    if n >= m:
        for i in range(m):
            bx[i] = wx[i]
            by[i] = wy[i]
    lines = [f"{m} {n} {k}"]
    lines += [f"{wx[i]} {wy[i]}" for i in range(m)]
    lines += [f"{bx[i]} {by[i]}" for i in range(n)]
    return "\n".join(lines) + "\n"


def stress():
    rng = random.Random(20261016)
    valid = 0
    while valid < ROUNDS:
        inp = gen(rng)
        gb = int(run(EXE_BRUTE, inp))
        if gb < 0:  # 随机图无解则重抽(题面保证必然有解)
            continue
        gm = int(run(EXE_MAIN, inp))
        if gm != gb:
            print(f"对拍不一致!\n{inp}标程: {gm} | 暴力: {gb}")
            sys.exit(1)
        valid += 1
    print(f"随机对拍: {ROUNDS} 组有解实例全部一致")


def worst_case_timing():
    rng = random.Random(99)
    m = n = 100
    k = 3000
    wx = [rng.randint(0, 5000) for _ in range(m)]
    wy = [rng.randint(0, 5000) for _ in range(m)]
    bx = [rng.randint(0, 5000) for _ in range(n)]
    by = [rng.randint(0, 5000) for _ in range(n)]
    lines = [f"{m} {n} {k}"] + [f"{wx[i]} {wy[i]}" for i in range(m)] + [f"{bx[i]} {by[i]}" for i in range(n)]
    inp = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    out = run(EXE_MAIN, inp)
    dt = time.perf_counter() - t0
    print(f"最坏规模 (M=n=100): 输出 {out}, 耗时 {dt:.4f} s")


def main():
    build()
    boundary_cases()
    stress()
    worst_case_timing()
    print("全部验证通过")


if __name__ == "__main__":
    main()
