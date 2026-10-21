#!/usr/bin/env python3
import os, random, subprocess, sys, time

here = os.path.dirname(os.path.abspath(__file__))
std = '/tmp/P1832'
brute = '/tmp/P1832_brute'


def compile_all():
    for src, out in (('P1832.cpp', std), ('P1832_brute.cpp', brute)):
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


def py_dp(n):
    comp = [False] * (n + 1)
    pr = []
    for i in range(2, n + 1):
        if not comp[i]:
            pr.append(i)
        for p in pr:
            if i * p > n:
                break
            comp[i * p] = True
            if i % p == 0:
                break
    dp = [0] * (n + 1)
    dp[0] = 1
    for p in pr:
        for j in range(p, n + 1):
            dp[j] += dp[j - p]
    return dp[n]


def main():
    compile_all()
    tests = 0
    fixed = [('7\n', '3'), ('20\n', '26'), ('1\n', '0'), ('2\n', '1'), ('4\n', '1'), ('10\n', '5')]
    for inp, exp in fixed:
        a, b = run(std, inp), run(brute, inp)
        if a != exp or b != exp:
            print(f'FAIL fixed {inp.strip()} exp={exp} std={a} brute={b}')
            return 1
        tests += 1
    rnd = random.Random(1832)
    t0 = time.time()
    for i in range(1, 61):
        n = rnd.randint(1, 45)
        inp = f'{n}\n'
        a, b = run(std, inp), run(brute, inp)
        exp = str(py_dp(n))
        if a != b or a != exp:
            print(f'FAIL n={n} std={a} brute={b} py={exp}')
            return 1
        tests += 1
    dt = time.time() - t0
    print(f'[随机+边界] {tests} 组一致,随机耗时 {dt:.2f}s')
    t1 = time.time()
    ans = run(std, '1000\n', timeout=10)
    tm = time.time() - t1
    exp = str(py_dp(1000))
    if ans != exp:
        print(f'FAIL n=1000 std={ans} py={exp}')
        return 1
    print(f'[最大规模] n=1000 ans={ans} time={tm:.4f}s')
    tests += 1
    print(f'{tests} tests OK')
    return 0


if __name__ == '__main__':
    sys.exit(main())
