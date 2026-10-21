#!/usr/bin/env python3
import os, random, subprocess, sys, time

here = os.path.dirname(os.path.abspath(__file__))
std = '/tmp/P1830'
brute = '/tmp/P1830_brute'
sample = """10 10 2 3
1 1 5 5
5 5 10 10
3 2
5 5
7 1
"""
sample_out = """Y 1 1
Y 2 2
N"""


def compile_all():
    for src, out in (('P1830.cpp', std), ('P1830_brute.cpp', brute)):
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


def gen(rnd):
    n = rnd.randint(1, 20)
    m = rnd.randint(1, 20)
    x = rnd.randint(0, 25)
    y = rnd.randint(1, 25)
    lines = [f'{n} {m} {x} {y}']
    for _ in range(x):
        x1, x2 = rnd.randint(1, n), rnd.randint(1, n)
        y1, y2 = rnd.randint(1, m), rnd.randint(1, m)
        if rnd.random() < 0.3:
            x1, x2 = x2, x1
        if rnd.random() < 0.3:
            y1, y2 = y2, y1
        lines.append(f'{x1} {y1} {x2} {y2}')
    for _ in range(y):
        lines.append(f'{rnd.randint(1, n)} {rnd.randint(1, m)}')
    return '\n'.join(lines) + '\n'


def main():
    compile_all()
    tests = 0
    got, bgot = run(std, sample), run(brute, sample)
    if got != sample_out or bgot != sample_out:
        print(f'sample FAIL\nstd=\n{got}\nbrut=\n{bgot}')
        return 1
    tests += 1
    rnd = random.Random(1830)
    t0 = time.time()
    for i in range(1, 301):
        inp = gen(rnd)
        a, b = run(std, inp), run(brute, inp)
        if a != b:
            print(f'FAIL round {i}\n{inp}std=\n{a}\nbrut=\n{b}')
            return 1
        tests += 1
    dt = time.time() - t0
    edges = [
        '1 1 0 1\n1 1\n',
        '3 3 1 1\n3 3 1 1\n2 2\n',
        '5 5 3 2\n1 1 5 5\n1 1 1 1\n2 2 4 4\n1 1\n5 5\n',
        '2 2 2 1\n1 2 2 1\n2 2 2 2\n2 2\n',
    ]
    for inp in edges:
        a, b = run(std, inp), run(brute, inp)
        if a != b:
            print(f'FAIL edge\n{inp}std=\n{a}\nbrut=\n{b}')
            return 1
        tests += 1
    print(f'[随机+边界] {tests} 组一致,随机耗时 {dt:.2f}s')
    # 满图轰炸，只计时标程
    n = m = 100
    x = 200
    y = 100
    er = random.Random(30)
    lines = [f'{n} {m} {x} {y}']
    for _ in range(x):
        lines.append(f'1 1 {n} {m}')
    for _ in range(y):
        lines.append(f'{er.randint(1, n)} {er.randint(1, m)}')
    big = '\n'.join(lines) + '\n'
    t1 = time.time()
    ans = run(std, big, timeout=20)
    tm = time.time() - t1
    rows = ans.splitlines()
    if len(rows) != y or any(not row.startswith(f'Y {x} {x}') for row in rows):
        print(f'FAIL max\n{ans[:200]}')
        return 1
    print(f'[最大规模] n=m=100 x=200 y=100 time={tm:.4f}s')
    tests += 1
    print(f'{tests} tests OK')
    return 0


if __name__ == '__main__':
    sys.exit(main())
