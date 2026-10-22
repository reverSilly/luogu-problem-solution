#!/usr/bin/env python3
# P2040 对拍：BFS 标程 vs 子集枚举暴力
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
    build('P2040.cpp', 'P2040')
    build('P2040_brute.cpp', 'P2040_brute')
    cases = []
    for _ in range(300):
        b = [random.randint(0, 1) for _ in range(9)]
        cases.append(' '.join(map(str, b)))
    # 极端构造
    cases += [' '.join(['0'] * 9), ' '.join(['1'] * 9),
              '0 1 1 1 0 0 1 0 1', '1 0 0 0 1 0 0 0 1']
    bad = 0
    for c in cases:
        a, b = run('P2040', c), run('P2040_brute', c)
        if a != b:
            bad += 1
            print('MISMATCH', c, 'sol=', a, 'brute=', b)
    print('P2040 done, cases=%d mismatches=%d' % (len(cases), bad))

if __name__ == '__main__':
    main()
