#!/usr/bin/env python3
import os, random, subprocess, sys, time

here = os.path.dirname(os.path.abspath(__file__))
std = '/tmp/P1821'
brute = '/tmp/P1821_brute'
sample = """4 8 2
1 2 4
1 3 2
1 4 7
2 1 1
2 3 5
3 1 2
3 4 4
4 2 3
"""


def compile_all():
    for src, out in (('P1821.cpp', std), ('P1821_brute.cpp', brute)):
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
    n = rnd.randint(1, 45)
    x = rnd.randint(1, n)
    edges = []
    for i in range(1, n + 1):
        if i == x:
            continue
        edges.append((i, x, rnd.randint(1, 100)))
        edges.append((x, i, rnd.randint(1, 100)))
    extra = rnd.randint(0, n * 3)
    for _ in range(extra):
        u, v = rnd.randint(1, n), rnd.randint(1, n)
        edges.append((u, v, rnd.randint(1, 100)))
    if not edges:
        edges.append((1, 1, rnd.randint(1, 100)))
    lines = [f'{n} {len(edges)} {x}']
    lines += [f'{u} {v} {w}' for u, v, w in edges]
    return '\n'.join(lines) + '\n'


def main():
    compile_all()
    tests = 0
    got = run(std, sample)
    bgot = run(brute, sample)
    if got != '10' or bgot != '10':
        print(f'sample FAIL std={got} brute={bgot}')
        return 1
    tests += 1
    rnd = random.Random(1821)
    t0 = time.time()
    rounds = 200
    for i in range(1, rounds + 1):
        inp = gen(rnd)
        a, b = run(std, inp), run(brute, inp)
        if a != b:
            print(f'FAIL round {i}\n{inp}std={a}\nbrut={b}')
            return 1
        tests += 1
    dt = time.time() - t0
    edges = [
        '1 1 1\n1 1 5\n',
        '2 2 1\n1 2 3\n2 1 4\n',
        '3 6 2\n1 2 1\n2 1 1\n2 3 1\n3 2 1\n1 3 100\n3 1 100\n',
    ]
    for inp in edges:
        a, b = run(std, inp), run(brute, inp)
        if a != b:
            print(f'FAIL edge\n{inp}std={a}\nbrut={b}')
            return 1
        tests += 1
    print(f'[随机+边界] {tests} 组一致,随机耗时 {dt:.2f}s')
    n, x = 1000, 500
    lines = []
    er = random.Random(7)
    for i in range(1, n + 1):
        if i != x:
            lines.append(f'{i} {x} 1')
            lines.append(f'{x} {i} {er.randint(1, 100)}')
    while len(lines) < 100000:
        lines.append(f'{er.randint(1, n)} {er.randint(1, n)} {er.randint(1, 100)}')
    big = f'{n} {len(lines)} {x}\n' + '\n'.join(lines) + '\n'
    t1 = time.time()
    ans = run(std, big, timeout=30)
    tm = time.time() - t1
    print(f'[最大规模] n=1000 m=100000 ans={ans} time={tm:.4f}s')
    tests += 1
    print(f'{tests} tests OK')
    return 0


if __name__ == '__main__':
    sys.exit(main())
