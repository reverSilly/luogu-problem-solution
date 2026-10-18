#!/usr/bin/env python3
"""P1512 [ICPC 2001 Taejon R] 伊甸园日历游戏 全量 39,023 个合法日期对拍与计时脚本"""
import os
import subprocess
import sys
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC_MAIN = os.path.join(BASE, "P1512.cpp")
SRC_BRUTE = os.path.join(BASE, "P1512_brute.cpp")
EXE_MAIN = os.path.join(BASE, "P1512")
EXE_BRUTE = os.path.join(BASE, "P1512_brute")
CXX = ["g++", "-O2", "-std=c++17"]

SAMPLE_IN = """3
2001 11 3
2001 11 2
2001 10 3
"""
SAMPLE_OUT = "YES\nNO\nNO"


def is_leap(y):
    return y % 400 == 0 or (y % 4 == 0 and y % 100 != 0)


def dim(y, m):
    if m in (1, 3, 5, 7, 8, 10, 12):
        return 31
    if m in (4, 6, 9, 11):
        return 30
    return 29 if is_leap(y) else 28


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
    print(f"官方样例回归: 标程={gm.splitlines()} | 暴力={gb.splitlines()}")
    assert gm == SAMPLE_OUT and gb == SAMPLE_OUT, "官方样例未通过"


def exhaustive_all_dates():
    dates = []
    for y in range(1900, 2007):
        for m in range(1, 13):
            for d in range(1, dim(y, m) + 1):
                if (y, m, d) >= (2006, 11, 4):
                    break
                dates.append((y, m, d))
            if (y, m) >= (2006, 11):
                break
    lines = [str(len(dates))] + [f"{y} {m} {d}" for y, m, d in dates]
    inp = "\n".join(lines) + "\n"
    t0 = time.perf_counter()
    gm = run(EXE_MAIN, inp)
    dt = time.perf_counter() - t0
    gb = run(EXE_BRUTE, inp)
    if gm != gb:
        out_m = gm.splitlines()
        out_b = gb.splitlines()
        for idx, (a, b) in enumerate(zip(out_m, out_b)):
            if a != b:
                print(f"不一致日期 {dates[idx]}: 标程={a}, 闭式={b}")
                sys.exit(1)
    print(f"全量穷举对拍: 1900.1.1 至 2006.11.3 共 {len(dates)} 个合法日期 100% 一致, 耗时 {dt:.4f} s")


def boundary_cases():
    cases = [
        ("起点 1900.1.1 (偶数)", "1\n1900 1 1\n", "YES"),
        ("平年2月底 1900.2.28 (百年不闰)", "1\n1900 2 28\n", "YES"),
        ("闰年2月28 2000.2.28 (四百年闰)", "1\n2000 2 28\n", "YES"),
        ("闰年2月29 2000.2.29 (奇数必败)", "1\n2000 2 29\n", "NO"),
        ("反常必胜点 9.30", "1\n2005 9 30\n", "YES"),
        ("反常必胜点 11.30", "1\n2005 11 30\n", "YES"),
        ("小月非反常 4.30 / 6.30", "2\n2005 4 30\n2005 6 30\n", "YES\nYES"),
        ("终点前一天 2006.11.3", "1\n2006 11 3\n", "YES"),
        ("终点前一月 2006.10.4", "1\n2006 10 4\n", "YES"),
        ("终点前封锁区 2006.10.5 (加月越界)", "1\n2006 10 5\n", "NO"),
    ]
    for name, inp, exp in cases:
        gm = run(EXE_MAIN, inp)
        gb = run(EXE_BRUTE, inp)
        assert gm == exp and gb == exp, f"边界失败 {name}: gm={gm}, gb={gb}, exp={exp}"
        print(f"边界通过: {name:<36s} -> {gm.replace(chr(10), '/')}")


def main():
    build()
    sample_case()
    boundary_cases()
    exhaustive_all_dates()
    print("全部验证通过")


if __name__ == "__main__":
    main()
