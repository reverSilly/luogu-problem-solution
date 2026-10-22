#!/usr/bin/env python3
# P2080 对拍：差值 DP 标程 vs 子集枚举暴力
import os, random, subprocess

D = os.path.dirname(os.path.abspath(__file__))

def build(src, exe):
    sp, ep = os.path.join(D, src), os.path.join(D, exe)
    if (not os.path.exists(ep)) or os.path.getmtime(ep) < os.path.getmtime(sp):
        subprocess.run(['g++', '-O2', '-std=c++17', sp, '-o', ep], check=True)

def run(exe, inp):
    p = subprocess.run([os.path.join(D, exe)], input=inp, text=True, capture_output=True)
    return p.stdout.strip()

def gen(rnd):
    n = rnd.randint(1, 12)
    v = rnd.randint(1, 60)
    lines = ["%d %d" % (n, v)]
    for _ in range(n):
        a = rnd.randint(-8, 8)
        b = rnd.randint(-8, 8)
        if a == 0:
            a = 3
        if b == 0:
            b = -3
        lines.append("%d %d" % (a, b))
    return '\n'.join(lines) + '\n'

def main():
    build('P2080.cpp', 'P2080')
    build('P2080_brute.cpp', 'P2080_brute')
    rnd = random.Random(20800001)
    cases = [gen(rnd) for _ in range(2000)]
    # 极端构造：全正 / 全负 / 刚好差一点 / 完全达不到
    cases.append("3 20\n5 5\n5 5\n5 5\n")
    cases.append("3 20\n-5 -5\n-5 -5\n-5 -5\n")
    cases.append("4 15\n5 6\n-1 8\n7 2\n1 0\n")
    cases.append("2 100\n10 10\n10 10\n")
    cases.append("2 5\n-3 3\n3 -3\n")
    bad = 0
    for c in cases:
        a, b = run('P2080', c), run('P2080_brute', c)
        if a != b:
            bad += 1
            print('MISMATCH\n' + c)
            print('sol=', a, 'brute=', b)
            if bad >= 5:
                break
    print('P2080 done, cases=%d mismatches=%d' % (len(cases), bad))

if __name__ == '__main__':
    main()
