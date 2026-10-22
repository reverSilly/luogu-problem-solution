#!/usr/bin/env python3
# P2074 对拍：局部窗口枚举标程 vs 全网格浮点开方暴力
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
    m = rnd.randint(1, 12)
    k = rnd.randint(1, 8)
    t = rnd.randint(0, 6)
    pts = []
    for _ in range(k):
        pts.append((rnd.randint(1, n), rnd.randint(1, m)))
    lines = ["%d %d %d %d" % (n, m, k, t)]
    for x, y in pts:
        lines.append("%d %d" % (x, y))
    return '\n'.join(lines) + '\n'

def main():
    build('P2074.cpp', 'P2074')
    build('P2074_brute.cpp', 'P2074_brute')
    rnd = random.Random(20740001)
    cases = [gen(rnd) for _ in range(2000)]
    # 极端构造：t=0 / 炸弹在角落 / 炸弹在正中间
    cases.append("3 3 1 0\n2 2\n")
    cases.append("5 5 1 3\n1 1\n")
    cases.append("6 6 2 2\n1 1\n6 6\n")
    cases.append("4 5 3 2\n1 2\n3 4\n4 5\n")
    bad = 0
    for c in cases:
        a, b = run('P2074', c), run('P2074_brute', c)
        if a != b:
            bad += 1
            print('MISMATCH\n' + c)
            print('sol=', a, 'brute=', b)
            if bad >= 5:
                break
    print('P2074 done, cases=%d mismatches=%d' % (len(cases), bad))

if __name__ == '__main__':
    main()
