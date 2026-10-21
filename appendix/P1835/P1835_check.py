#!/usr/bin/env python3
import os, random, subprocess, sys, time

here = os.path.dirname(os.path.abspath(__file__))
std = '/tmp/P1835'
brute = '/tmp/P1835_brute'


def compile_all():
    for src, out in (('P1835.cpp', std), ('P1835_brute.cpp', brute)):
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


def main():
    compile_all()
    tests = 0
    fixed = [
        (2, 11, '5'),
        (1, 1, '0'),
        (1, 2, '1'),
        (1, 10, '4'),
        (2, 2, '1'),
        (4, 4, '0'),
        (8, 10, '0'),
        (9, 9, '0'),
        (14, 16, '0'),
        (13, 17, '2'),
        (25, 25, '0'),
        (49, 49, '0'),
        (1, 100, None),
    ]
    t0 = time.time()
    for L, R, exp in fixed:
        inp = f'{L} {R}\n'
        a, b = run(std, inp), run(brute, inp)
        if a != b or (exp is not None and a != exp):
            print(f'FAIL [{L},{R}] std={a} brute={b} exp={exp}')
            return 1
        tests += 1
    rnd = random.Random(1835)
    for i in range(1, 41):
        if rnd.random() < 0.5:
            L = rnd.randint(1, 2000)
            R = rnd.randint(L, min(L + rnd.randint(0, 3000), 5000))
        else:
            R = rnd.randint(1, 200000)
            L = rnd.randint(max(1, R - rnd.randint(0, 1500)), R)
        inp = f'{L} {R}\n'
        a, b = run(std, inp), run(brute, inp)
        if a != b:
            print(f'FAIL round {i} [{L},{R}] std={a} brute={b}')
            return 1
        tests += 1
    dt = time.time() - t0
    print(f'[随机+边界] {tests} 组一致,耗时 {dt:.2f}s')
    # 端点素数：2^31-1 是梅森素数
    specials = [
        (2147483647, 2147483647, '1'),
        (2147483646, 2147483647, '1'),
        (2147483647 - 10, 2147483647, None),
    ]
    for L, R, exp in specials:
        inp = f'{L} {R}\n'
        a = run(std, inp)
        if exp is not None and a != exp:
            print(f'FAIL special [{L},{R}] std={a} exp={exp}')
            return 1
        tests += 1
        print(f'[端点] [{L},{R}] ans={a}')
    L = 2147483647 - 1000000
    R = 2147483647
    t1 = time.time()
    ans = run(std, f'{L} {R}\n', timeout=30)
    tm = time.time() - t1
    print(f'[最大规模] L={L} R={R} len={R - L} ans={ans} time={tm:.4f}s')
    # 再计一次区间长度打满、左端为 1
    t2 = time.time()
    ans2 = run(std, '1 1000001\n', timeout=30)
    tm2 = time.time() - t2
    if ans2 != run(brute, '1 1000001\n', timeout=60):
        print('FAIL [1,1000001] vs brute')
        return 1
    print(f'[最长小端] [1,1000001] ans={ans2} time={tm2:.4f}s')
    tests += 2
    print(f'{tests} tests OK')
    return 0


if __name__ == '__main__':
    sys.exit(main())
