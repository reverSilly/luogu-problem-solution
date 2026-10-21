#!/usr/bin/env python3
import os, random, subprocess, sys, time

here = os.path.dirname(os.path.abspath(__file__))
std = '/tmp/P1828'
brute = '/tmp/P1828_brute'
sample = """3 4 5
2
3
4
1 2 1
1 3 5
2 3 7
2 4 3
3 4 5
"""


def compile_all():
    for src, out in (('P1828.cpp', std), ('P1828_brute.cpp', brute)):
        r = subprocess.run(
            ['g++', '-O2', '-std=c++17', '-Wall', '-Wextra', '-Werror',
             '-o', out, os.path.join(here, src)],
            capture_output=True, text=True)
        if r.returncode != 0 or r.stderr.strip():
            sys.stderr.write(r.stderr)
            sys.exit(1)


def run(prog, inp, timeout=60):
    r = subprocess.run([prog], input=inp, capture_output=True, text=True, timeout=timeout)
    if r.returncode != 0:
        raise RuntimeError(f'{prog} exit {r.returncode}\n{r.stderr[:400]}')
    return r.stdout.strip()


def gen(rnd):
    p = rnd.randint(2, 28)
    n = rnd.randint(1, 40)
    edges = []
    for i in range(2, p + 1):
        j = rnd.randint(1, i - 1)
        edges.append((i, j, rnd.randint(1, 255)))
    extra = rnd.randint(0, p)
    for _ in range(extra):
        a, b = rnd.randint(1, p), rnd.randint(1, p)
        if a == b:
            b = a % p + 1
        edges.append((a, b, rnd.randint(1, 255)))
    cows = [rnd.randint(1, p) for _ in range(n)]
    lines = [f'{n} {p} {len(edges)}']
    lines += [str(c) for c in cows]
    lines += [f'{a} {b} {d}' for a, b, d in edges]
    return '\n'.join(lines) + '\n'


def main():
    compile_all()
    tests = 0
    got, bgot = run(std, sample), run(brute, sample)
    if got != '8' or bgot != '8':
        print(f'sample FAIL std={got} brute={bgot}')
        return 1
    tests += 1
    rnd = random.Random(1828)
    t0 = time.time()
    for i in range(1, 121):
        inp = gen(rnd)
        a, b = run(std, inp), run(brute, inp)
        if a != b:
            print(f'FAIL round {i}\n{inp}std={a}\nbrut={b}')
            return 1
        tests += 1
    dt = time.time() - t0
    edges = [
        '1 2 1\n1\n1 2 5\n',
        '4 3 2\n1\n1\n2\n3\n1 2 4\n2 3 6\n',
        '2 2 1\n1\n2\n1 2 9\n',
    ]
    for inp in edges:
        a, b = run(std, inp), run(brute, inp)
        if a != b:
            print(f'FAIL edge\n{inp}std={a}\nbrut={b}')
            return 1
        tests += 1
    print(f'[随机+边界] {tests} 组一致,随机耗时 {dt:.2f}s')
    P, N, C = 800, 500, 1450
    er = random.Random(28)
    edges = []
    for i in range(2, P + 1):
        edges.append((i, er.randint(1, i - 1), er.randint(1, 255)))
    while len(edges) < C:
        a, b = er.randint(1, P), er.randint(1, P)
        if a == b:
            continue
        edges.append((a, b, er.randint(1, 255)))
    cows = [er.randint(1, P) for _ in range(N)]
    lines = [f'{N} {P} {len(edges)}']
    lines += [str(c) for c in cows]
    lines += [f'{a} {b} {d}' for a, b, d in edges]
    big = '\n'.join(lines) + '\n'
    t1 = time.time()
    ans = run(std, big, timeout=60)
    tm = time.time() - t1
    print(f'[最大规模] N=500 P=800 C=1450 ans={ans} time={tm:.4f}s')
    tests += 1
    print(f'{tests} tests OK')
    return 0


if __name__ == '__main__':
    sys.exit(main())
