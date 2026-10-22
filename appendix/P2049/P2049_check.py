#!/usr/bin/env python3
# P2049 对拍：余数集合 DP 标程 vs 全路径枚举暴力
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
    build('P2049.cpp', 'P2049')
    build('P2049_brute.cpp', 'P2049_brute')
    cases = []
    for _ in range(300):
        m = random.randint(1, 4)
        n = random.randint(1, 4)
        k = random.randint(1, 12)
        cells = [random.randint(0, 30) for _ in range(m * n)]
        cases.append('%d %d %d\n%s' % (m, n, k, '\n'.join(' '.join(map(str, cells[i * n:(i + 1) * n])) for i in range(m))))
    # 极端构造：全 0、全 1、含 0 的格子、K=1
    cases.append('1 1 1\n0')
    cases.append('2 2 5\n1 1\n1 1')
    cases.append('2 2 5\n0 3\n3 0')
    cases.append('1 5 7\n1 2 3 4 5')
    bad = 0
    for c in cases:
        a, b = run('P2049', c), run('P2049_brute', c)
        if a != b:
            bad += 1
            print('MISMATCH\n' + c, 'sol=', repr(a), 'brute=', repr(b))
    print('P2049 done, cases=%d mismatches=%d' % (len(cases), bad))

if __name__ == '__main__':
    main()
