#!/usr/bin/env python3
# P2062 对拍：块 DP 标程 vs 状态压缩暴力
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
    build('P2062.cpp', 'P2062')
    build('P2062_brute.cpp', 'P2062_brute')
    cases = []
    for _ in range(300):
        n = random.randint(1, 10)
        cases.append('%d\n%s' % (n, '\n'.join(str(random.randint(1, n)) for _ in range(n))))
    # 极端构造：全 1、全 n、混合
    cases.append('5\n2\n1\n2\n2\n3')
    cases.append('4\n1\n1\n1\n1')
    cases.append('4\n4\n4\n4\n4')
    cases.append('6\n1\n2\n3\n4\n5\n6')
    bad = 0
    for c in cases:
        a, b = run('P2062', c), run('P2062_brute', c)
        if a != b:
            bad += 1
            print('MISMATCH\n' + c, 'sol=', a, 'brute=', b)
    print('P2062 done, cases=%d mismatches=%d' % (len(cases), bad))

if __name__ == '__main__':
    main()
