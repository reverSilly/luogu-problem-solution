#!/usr/bin/env python3
# P1871 对拍：质因数占用表 vs 扫描已开启编号求 gcd。冲突编号允许任意一台。
import math
import os
import random
import subprocess
import sys
import tempfile
import time

BASE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.join(BASE, "P1871.cpp")
BRU = os.path.join(BASE, "P1871_brute.cpp")
EXE = os.path.join(tempfile.gettempdir(), "P1871")
EXEB = os.path.join(tempfile.gettempdir(), "P1871_brute")
CXX = ["g++", "-O2", "-std=c++17", "-Wall", "-Wextra"]

SAMPLES = [
"""10 10
+ 6
+ 10
+ 5
- 10
- 5
- 6
+ 10
+ 3
+ 6
+ 3
"""
]


def build():
    for src, out in ((SRC, EXE), (BRU, EXEB)):
        r = subprocess.run(CXX + [src, "-o", out], capture_output=True, text=True)
        if r.returncode != 0 or r.stderr.strip():
            print("编译失败或有警告:", src)
            print(r.stderr)
            return 1
    return 0


def run(exe, data):
    r = subprocess.run([exe], input=data.encode(), capture_output=True)
    if r.returncode != 0:
        print("运行失败", exe, r.stderr.decode())
        return None
    return r.stdout.decode()


def judge(data, out):
    lines = data.strip().splitlines()
    n, m = map(int, lines[0].split())
    ops = lines[1:]
    res = out.splitlines()
    if len(ops) != m or len(res) != m:
        return False, "行数不符"
    on = set()
    for k in range(m):
        op, xs = ops[k].split()
        x = int(xs)
        line = res[k]
        if op == "+":
            if x in on:
                if line != "Already on":
                    return False, f"期望 Already on 得到 {line}"
            else:
                conf = [j for j in on if math.gcd(j, x) > 1]
                if conf:
                    pref = "Conflict with "
                    if not line.startswith(pref):
                        return False, line
                    try:
                        j = int(line[len(pref):])
                    except ValueError:
                        return False, line
                    if j not in conf:
                        return False, f"冲突编号 {j} 非法, 合法={sorted(conf)}"
                else:
                    if line != "Success":
                        return False, f"期望 Success 得到 {line}"
                    on.add(x)
        else:
            if x not in on:
                if line != "Already off":
                    return False, f"期望 Already off 得到 {line}"
            else:
                if line != "Success":
                    return False, f"期望 Success 得到 {line}"
                on.remove(x)
    return True, ""


def gen(rng):
    n = rng.randint(1, 350)
    m = rng.randint(1, 350)
    ops = [f"{n} {m}"]
    for _ in range(m):
        x = rng.randint(1, n)
        op = "+" if rng.random() < 0.7 else "-"
        ops.append(f"{op} {x}")
    return "\n".join(ops) + "\n"


def main():
    if build():
        return 1
    rng = random.Random(1871)
    cases = list(SAMPLES)
    cases.append("1 3\n+ 1\n+ 1\n- 1\n")
    cases.append("8 4\n+ 1\n+ 8\n- 1\n+ 6\n")
    for _ in range(160):
        cases.append(gen(rng))
    for i, data in enumerate(cases):
        a = run(EXE, data)
        b = run(EXEB, data)
        if a is None or b is None:
            print("FAIL run", i)
            print(data)
            return 1
        ok_a, msg_a = judge(data, a)
        ok_b, msg_b = judge(data, b)
        if not ok_a or not ok_b:
            print("MISMATCH", i, msg_a, msg_b)
            print(data)
            print("std:", a)
            print("brute:", b)
            return 1
    big = ["100000 100000"]
    rng2 = random.Random(1)
    state = [False] * 100001
    for i in range(100000):
        x = rng2.randint(1, 100000)
        if state[x] and rng2.random() < 0.5:
            big.append(f"- {x}")
            state[x] = False
        else:
            big.append(f"+ {x}")
            state[x] = True
    data = "\n".join(big) + "\n"
    t0 = time.perf_counter()
    a = run(EXE, data)
    dt = time.perf_counter() - t0
    if a is None or len(a.splitlines()) != 100000:
        print("大样例失败")
        return 1
    print(f"MAX n=m=100000 time={dt:.4f}s")
    print(f"{len(cases)} tests OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
