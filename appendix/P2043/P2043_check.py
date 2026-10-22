#!/usr/bin/env python3
# P2043 对拍：Legendre 标程 vs 逐数试除暴力
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
    build('P2043.cpp', 'P2043')
    build('P2043_brute.cpp', 'P2043_brute')
    cases = [str(random.randint(1, 10000)) for _ in range(300)]
    cases += ['1', '2', '3', '4', '5', '10', '100', '1000', '9999', '10000']
    bad = 0
    for c in cases:
        a, b = run('P2043', c), run('P2043_brute', c)
        if a != b:
            bad += 1
            print('MISMATCH N=', c, 'sol=', a, 'brute=', b)
    print('P2043 done, cases=%d mismatches=%d' % (len(cases), bad))

if __name__ == '__main__':
    main()
