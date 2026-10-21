#!/usr/bin/env python3
import os, random, subprocess, sys, time

here = os.path.dirname(os.path.abspath(__file__))
std = '/tmp/P1833'
brute = '/tmp/P1833_brute'
sample = """6:50 7:00 3
2 1 0
3 3 1
4 5 4
"""


def compile_all():
    for src, out in (('P1833.cpp', std), ('P1833_brute.cpp', brute)):
        r = subprocess.run(
            ['g++', '-O2', '-std=c++17', '-Wall', '-Wextra', '-Werror',
             '-o', out, os.path.join(here, src)],
            capture_output=True, text=True)
        if r.returncode != 0 or r.stderr.strip():
            sys.stderr.write(r.stderr)
            sys.exit(1)


def run(prog, inp, timeout=20):
    r = subprocess.run([prog], input=inp, capture_output=True, text=True, timeout=timeout)
    if r.returncode != 0:
        raise RuntimeError(f'{prog} exit {r.returncode}\n{r.stderr[:400]}')
    return r.stdout.strip()


def fmt_time(mins):
    return f'{mins // 60}:{mins % 60:02d}'


def gen(rnd):
    T = rnd.randint(0, 12)
    ts = rnd.randint(0, 23 * 60 + 59 - max(T, 0))
    te = ts + T
    n = rnd.randint(0, 4)
    lines = [f'{fmt_time(ts)} {fmt_time(te)} {n}']
    for _ in range(n):
        t = rnd.randint(1, max(1, T if T else 1))
        c = rnd.randint(1, 20)
        if rnd.random() < 0.25:
            p = 0
        else:
            p = rnd.randint(1, 4)
        lines.append(f'{t} {c} {p}')
    return '\n'.join(lines) + '\n'


def main():
    compile_all()
    tests = 0
    got, bgot = run(std, sample), run(brute, sample)
    if got != '11' or bgot != '11':
        print(f'sample FAIL std={got} brute={bgot}')
        return 1
    tests += 1
    rnd = random.Random(1833)
    t0 = time.time()
    for i in range(1, 251):
        inp = gen(rnd)
        a, b = run(std, inp), run(brute, inp)
        if a != b:
            print(f'FAIL round {i}\n{inp}std={a}\nbrut={b}')
            return 1
        tests += 1
    dt = time.time() - t0
    edges = [
        '0:00 0:00 0\n',
        '8:00 8:00 2\n1 5 3\n2 9 0\n',
        '0:00 0:10 1\n3 4 0\n',
        '1:05 1:06 1\n1 100 5\n',
        '12:00 12:20 3\n5 1 1\n5 100 0\n7 3 2\n',
        '23:50 23:59 2\n4 8 0\n9 20 1\n',
    ]
    for inp in edges:
        a, b = run(std, inp), run(brute, inp)
        if a != b:
            print(f'FAIL edge\n{inp}std={a}\nbrut={b}')
            return 1
        tests += 1
    print(f'[随机+边界] {tests} 组一致,随机耗时 {dt:.2f}s')
    # n=10000,T=1000,多重背包最坏形态
    lines = ['0:00 16:40 10000']
    for i in range(10000):
        lines.append(f'1 {1 + (i % 200)} 99')
    big = '\n'.join(lines) + '\n'
    t1 = time.time()
    ans = run(std, big, timeout=30)
    tm = time.time() - t1
    # 每分钟最多 200，时间 1000，理论 200000；物品价值循环覆盖 1..200 且次数足够
    if ans != '200000':
        print(f'FAIL max ans={ans}')
        return 1
    print(f'[最大规模] n=10000 T=1000 ans={ans} time={tm:.4f}s')
    tests += 1
    print(f'{tests} tests OK')
    return 0


if __name__ == '__main__':
    sys.exit(main())
