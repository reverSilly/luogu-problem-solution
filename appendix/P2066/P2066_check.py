#!/usr/bin/env python3
# P2066 对拍：分组背包 DP 标程 vs 全枚举暴力
import os, random, subprocess

D = os.path.dirname(os.path.abspath(__file__))

def build(src, exe):
    sp, ep = os.path.join(D, src), os.path.join(D, exe)
    if (not os.path.exists(ep)) or os.path.getmtime(ep) < os.path.getmtime(sp):
        subprocess.run(['g++', '-O2', '-std=c++17', sp, '-o', ep], check=True)

def run(exe, inp):
    p = subprocess.run([os.path.join(D, exe)], input=inp, text=True, capture_output=True)
    return p.stdout.strip()

def main():
    build('P2066.cpp', 'P2066')
    build('P2066_brute.cpp', 'P2066_brute')
    cases = []
    for _ in range(200):
        n = random.randint(1, 3)
        m = random.randint(1, 5)
        mat = [[random.randint(0, 50) for _ in range(m)] for _ in range(n)]
        cases.append('%d %d\n%s' % (n, m, '\n'.join(' '.join(map(str, row)) for row in mat)))
    # 极端构造：全 0、单调递增、存在并列最优
    cases.append('2 2\n0 0\n0 0')
    cases.append('2 3\n1 2 3\n1 2 3')
    cases.append('3 3\n30 40 50\n20 30 50\n20 25 30')
    bad = 0
    for c in cases:
        a, b = run('P2066', c), run('P2066_brute', c)
        if a != b:
            bad += 1
            print('MISMATCH\n' + c, 'sol=', repr(a), 'brute=', repr(b))
    print('P2066 done, cases=%d mismatches=%d' % (len(cases), bad))

if __name__ == '__main__':
    main()
