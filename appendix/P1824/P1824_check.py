#!/usr/bin/env python3
import os, random, subprocess, sys, time

here = os.path.dirname(os.path.abspath(__file__))
std = '/tmp/P1824'
brute = '/tmp/P1824_brute'
sample = """5 3
1
2
8
4
9
"""


def compile_all():
    for src, out in (('P1824.cpp', std), ('P1824_brute.cpp', brute)):
        r = subprocess.run(
            ['g++', '-O2', '-std=c++17', '-Wall', '-Wextra', '-Werror',
             '-o', out, os.path.join(here, src)],
            capture_output=True, text=True)
        if r.returncode != 0 or r.stderr.strip():
            sys.stderr.write(r.stderr)
            sys.exit(1)


def run(prog, inp, timeout=30):
    r = subprocess.run([prog], input=inp, capture_output=True, text=True, timeout=timeout)
    if r.returncode != 0:
        raise RuntimeError(f'{prog} exit {r.returncode}\n{r.stderr[:400]}')
    return r.stdout.strip()


def gen(rnd):
    n = rnd.randint(2, 16)
    m = rnd.randint(2, n)
    hi = rnd.choice([10, 20, 100, 10**9])
    xs = [rnd.randint(0, hi) for _ in range(n)]
    if rnd.random() < 0.15:
        xs = [xs[0]] * n
    body = '\n'.join(map(str, xs))
    return f'{n} {m}\n{body}\n'


def main():
    compile_all()
    tests = 0
    got, bgot = run(std, sample), run(brute, sample)
    if got != '3' or bgot != '3':
        print(f'sample FAIL std={got} brute={bgot}')
        return 1
    tests += 1
    rnd = random.Random(1824)
    t0 = time.time()
    for i in range(1, 301):
        inp = gen(rnd)
        a, b = run(std, inp), run(brute, inp)
        if a != b:
            print(f'FAIL round {i}\n{inp}std={a}\nbrut={b}')
            return 1
        tests += 1
    dt = time.time() - t0
    edges = [
        '2 2\n0\n0\n',
        '2 2\n0\n1000000000\n',
        '5 5\n5\n4\n3\n2\n1\n',
        '4 2\n7\n7\n7\n9\n',
        '6 3\n1\n1\n1\n1\n1\n4\n',
    ]
    for inp in edges:
        a, b = run(std, inp), run(brute, inp)
        if a != b:
            print(f'FAIL edge\n{inp}std={a}\nbrut={b}')
            return 1
        tests += 1
    print(f'[随机+边界] {tests} 组一致,随机耗时 {dt:.2f}s')
    n, m = 100000, 50000
    er = random.Random(24)
    xs = [er.randint(0, 10**9) for _ in range(n)]
    big = f'{n} {m}\n' + '\n'.join(map(str, xs)) + '\n'
    t1 = time.time()
    ans = run(std, big, timeout=30)
    tm = time.time() - t1
    print(f'[最大规模] n=100000 m=50000 ans={ans} time={tm:.4f}s')
    tests += 1
    print(f'{tests} tests OK')
    return 0


if __name__ == '__main__':
    sys.exit(main())
