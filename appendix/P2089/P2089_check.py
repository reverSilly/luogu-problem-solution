#!/usr/bin/env python3
# P2089 对拍：DFS 标程 vs 三进制枚举暴力
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
    build('P2089.cpp', 'P2089')
    build('P2089_brute.cpp', 'P2089_brute')
    rnd = random.Random(20260101)
    cases = [str(rnd.randint(1, 30)) for _ in range(2000)]
    cases += ['1', '10', '11', '20', '29', '30', '31', '100']
    bad = 0
    for c in cases:
        a, b = run('P2089', c), run('P2089_brute', c)
        if a != b:
            bad += 1
            print('MISMATCH n=%s\nsol:\n%s\nbrute:\n%s' % (c, a, b))
            if bad >= 5:
                break
    print('P2089 done, cases=%d mismatches=%d' % (len(cases), bad))

if __name__ == '__main__':
    main()
